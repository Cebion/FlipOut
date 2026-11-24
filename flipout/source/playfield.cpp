/* $Id$

This file is part of flipout
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include <cmath>
#include <typeinfo>
#include <vector>
#include <msp/debug/profilingscope.h>
#include <msp/gl/blend.h>
#include <msp/gl/except.h>
#include <msp/gl/immediate.h>
#include <msp/gl/matrix.h>
#include <msp/gl/misc.h>
#include <msp/gl/projection.h>
#include <msp/gl/tests.h>
#include <msp/gl/texture.h>
#include <msp/gl/texunit.h>
#include <msp/gl/transform.h>
#include <msp/strings/formatter.h>
#include <msp/time/units.h>
#include "award.h"
#include "background.h"
#include "ball.h"
#include "balltype.h"
#include "bonus.h"
#include "bonusaction.h"
#include "bonuseffect.h"
#include "bonustype.h"
#include "brick.h"
#include "bricktype.h"
#include "collider.h"
#include "flipout.h"
#include "glow.h"
#include "geomcategory.h"
#include "misc.h"
#include "paddle.h"
#include "playfield.h"
#include "shadow.h"
#include "reflect.h"
#include "wall.h"

using namespace std;
using namespace Msp;

Playfield::Playfield(FlipOut &fo, float w, float h):
	flipout(fo),
	width(w),
	height(h),
	gravity(8),
	background(0),
	world(dWorldCreate()),
	space(dHashSpaceCreate(0)),
	contacts(dJointGroupCreate(0)),
	moving_balls(0),
	n_bricks(0),
	balls_stuck(false),
	fill_bonus(0),
	ball_magnet(0),
	unlimited_balls(false),
	gravity_modifier(1),
	friction_modifier(1),
	quake_strength(0),
	quake_phase(0),
	juggler_level(1),
	bonuses_created(0),
	bonuses_collected(0),
	camera(width, height, M_PI/6, flipout.get_window_aspect()),
	glow(0),
	shadow(0),
	reflect(0),
	n_contacts(0)
{
	dHashSpaceSetLevels(space, -4, 2);

	set_gravity_modifier(1);
	dSpaceSetCleanup(space, 1);

	new Wall(*this, Wall::FLOOR);
	new Wall(*this, Wall::LEFT);
	new Wall(*this, Wall::RIGHT);
	new Wall(*this, Wall::TOP);

	dGeomID plane=dCreatePlane(space, 0, 1, 0, 2);
	dGeomSetCategoryBits(plane, 0);
	dGeomSetCollideBits(plane, BRICK);

	colliders.push_back(new Collider2<Ball, Brick>(sigc::mem_fun(this, &Playfield::collide_ball_brick)));
	colliders.push_back(new Collider2<Ball, Bonus>(sigc::mem_fun(this, &Playfield::collide_ball_bonus)));
	colliders.push_back(new Collider2<Ball, Ball>(sigc::mem_fun(this, &Playfield::collide_ball_ball)));
	colliders.push_back(new Collider2<Paddle, Bonus>(sigc::mem_fun(this, &Playfield::collide_paddle_bonus)));
	colliders.push_back(new Collider2<Paddle, Object>(sigc::mem_fun(this, &Playfield::collide_paddle_object)));
	colliders.push_back(new Collider2<Brick, Brick>(sigc::mem_fun(this, &Playfield::collide_brick_brick)));
	colliders.push_back(new Collider2<Brick, Bonus>(sigc::mem_fun(this, &Playfield::collide_brick_bonus)));
	colliders.push_back(new Collider2<Wall, Object>(sigc::mem_fun(this, &Playfield::collide_wall_object)));

	Vector3 ld(0, -0.3, 1);
	normalize(ld);

	light_diffuse.set_diffuse(GL::Color(0.6));
	light_diffuse.set_position(ld.x, ld.y, ld.z, 0);
	lighting_diffuse.attach(0, light_diffuse);
	lighting_diffuse.set_ambient(GL::Color(0.0));

	light_fill.set_diffuse(GL::Color(0.2));
	light_fill.set_specular(GL::Color(0.0));
	light_fill.set_position(ld.x, ld.y, ld.z, 0);
	lighting_fill.attach(0, light_fill);
	lighting_fill.set_ambient(GL::Color(0.3));

	light_combined.set_diffuse(GL::Color(0.8));
	light_combined.set_position(ld.x, ld.y, ld.z, 0);
	lighting_combined.attach(0, light_combined);
	lighting_combined.set_ambient(GL::Color(0.3));

	if(flipout.get_settings().shadows)
	{
		try
		{
			shadow=new Shadow(flipout, 1<<flipout.get_settings().shadow_quality, ld, max(width, height), -10, 5-ld.y*height);
		}
		catch(const GL::UnsupportedExtension &)
		{ }
	}

	const Resources &res=flipout.get_resources();
	if(res.contains("Immortal"))
		awards["Immortal"]=res.get<Award>("Immortal")->get_score(1);

	if(flipout.get_settings().glow && flipout.get_settings().use_shaders)
	{
		try
		{
			glow=new Glow(flipout);
		}
		catch(const GL::UnsupportedExtension &)
		{ }
	}
}

Playfield::~Playfield()
{
	// Zero the counters to avoid false signals
	balls.clear();
	n_bricks=0;

	while(!objects.empty())
		delete *objects.begin();

	for(list<BonusEffect *>::iterator i=effects.begin(); i!=effects.end(); ++i)
		delete *i;

	dJointGroupDestroy(contacts);
	dSpaceDestroy(space);
	dWorldDestroy(world);

	delete reflect;
	delete shadow;
	delete glow;

	for(list<Collider *>::iterator i=colliders.begin(); i!=colliders.end(); ++i)
		delete *i;
}

void Playfield::set_gravity(float g)
{
	gravity=g*pow(1.67, flipout.get_settings().difficulty-1.0);
	dWorldSetGravity(world, 0, 0, -gravity*gravity_modifier*2-1);
}

void Playfield::set_gravity_modifier(float m)
{
	gravity_modifier=m;
	dWorldSetGravity(world, 0, 0, -gravity*gravity_modifier*2-1);
	activate_objects();
}

void Playfield::set_friction_modifier(float f)
{
	friction_modifier=f;
	activate_objects();
}

void Playfield::set_background(const Background *bg)
{
	background=bg;

	if(bg->get_reflect()>0 && !reflect && flipout.get_settings().reflection)
	{
		try
		{
			reflect=new Reflect(flipout);
		}
		catch(const GL::UnsupportedExtension &)
		{ }
	}
}

void Playfield::set_fill_bonus(const BonusType *b)
{
	fill_bonus=b;
}

void Playfield::set_ball_magnet(double m)
{
	ball_magnet=m;
}

void Playfield::set_unlimited_balls(bool u)
{
	unlimited_balls=u;
}

void Playfield::set_quake(float q)
{
	quake_strength=q;
	quake_phase=0;
}

void Playfield::add(Object &o)
{
	objects.insert(&o);
}

void Playfield::add(Ball &b)
{
	objects.insert(&b);
	balls.insert(&b);
	b.signal_stuck.connect(sigc::mem_fun(this, &Playfield::ball_stuck));

	if(balls.size()>juggler_level)
		juggler_level=balls.size();
}

void Playfield::add(Brick &b)
{
	objects.insert(&b);
	if(!b.get_type().is_indestructible())
		++n_bricks;
}

void Playfield::add(Paddle &p)
{
	objects.insert(&p);
	paddle=&p;
}

void Playfield::add(Bonus &b)
{
	objects.insert(&b);
	bonuses.insert(&b);
	++bonuses_created;
}

void Playfield::remove(Object &o)
{
	objects.erase(&o);
}

void Playfield::remove(Ball &b)
{
	objects.erase(&b);
	bool had_balls=!balls.empty();
	balls.erase(&b);
	if(!b.is_stuck())
		--moving_balls;

	if(had_balls)
	{
		if(balls.empty())
		{
			if(!unlimited_balls)
			{
				destroy_bonuses();
				awards.erase("Immortal");
			}
			if(unlimited_balls || signal_lost_life.emit())
				place_ball(*new Ball(*this, *flipout.get_resources().get<BallType>("normal ball")));
		}
		else if(!moving_balls)
			balls_stuck=true;
	}
}

void Playfield::remove(Brick &b)
{
	objects.erase(&b);
	if(!--n_bricks)
	{
		const Resources &res=flipout.get_resources();

		if(bonuses_created>0 && res.contains("Collector"))
		{
			const Award &award=*res.get<Award>("Collector");
			unsigned collect_percent=bonuses_collected*100/bonuses_created;
			unsigned score=award.get_score(collect_percent);
			if(score>0)
				awards[format("Collector (%d%%)", collect_percent)]=score;
		}

		if(juggler_level>=2 && res.contains("Juggler"))
		{
			const Award &award=*res.get<Award>("Juggler");
			unsigned score=award.get_score(juggler_level);
			if(score>0)
				awards[format("Juggler (%d)", juggler_level)]=score;
		}

		signal_finished.emit();
	}
}

void Playfield::remove(Bonus &b)
{
	objects.erase(&b);
	bonuses.erase(&b);
}

void Playfield::destroy_bonuses()
{
	while(!bonuses.empty())
		delete *bonuses.begin();

	for(list<BonusEffect *>::iterator i=effects.begin(); i!=effects.end(); ++i)
	{
		(*i)->get_action().terminate(*this);
		delete *i;
	}
	effects.clear();
}

void Playfield::add_effect(const BonusType &bt, const BonusAction &a)
{
	for(list<BonusEffect *>::iterator i=effects.begin(); i!=effects.end();)
	{
		if(&(*i)->get_action()==&a)
		{
			(*i)->extend(bt.get_duration());
			return;
		}
		else if(typeid(a)==typeid((*i)->get_action()))
		{
			delete *i;
			i=effects.erase(i);
		}
		else
			++i;
	}

	effects.push_back(new BonusEffect(*this, bt, a));
}

void Playfield::place_ball(Ball &ball)
{
	paddle->attach(ball);
}

void Playfield::step(const Msp::Time::TimeDelta &stepsize)
{
	quake_scale=sqrt(0.01/(stepsize/Msp::Time::sec));

	for(set<Object *>::iterator i=objects.begin(); i!=objects.end(); ++i)
		(*i)->step(stepsize);

	if(balls_stuck)
	{
		if(!unlimited_balls)
		{
			list<Ball *> bls(balls.begin(), balls.end());
			balls.clear();
			moving_balls=0;
			for(list<Ball *>::iterator i=bls.begin(); i!=bls.end(); ++i)
				delete *i;
		}

		place_ball(*new Ball(*this, *flipout.get_resources().get<BallType>("normal ball")));
		balls_stuck=false;
	}

	for(list<BonusEffect *>::iterator i=effects.begin(); i!=effects.end(); ++i)
		(*i)->step(stepsize);

	{
		Debug::ProfilingScope _ps(flipout.get_profiler(), "collide");

		list<ContactInfo> ct;
		dSpaceCollide(space, &ct, &near_callback);
		n_contacts=0;

		for(list<ContactInfo>::iterator i=ct.begin(); i!=ct.end();)
		{
			if(collide(*i))
				++i;
			else
				i=ct.erase(i);
		}

		for(list<ContactInfo>::iterator i=ct.begin(); i!=ct.end(); ++i)
			if(!i->o1 || !i->o2 || i->o1->is_active() || i->o2->is_active())
				create_contacts(*i);
	}

	{
		Debug::ProfilingScope _ps(flipout.get_profiler(), "step");

		dWorldQuickStep(world, stepsize/Msp::Time::sec);
	}
	dJointGroupEmpty(contacts);

	if(quake_strength>0)
		quake_phase+=(stepsize/Msp::Time::sec)*60;

	list<Object *> stale;
	for(set<Object *>::iterator i=objects.begin(); i!=objects.end(); ++i)
		if((*i)->is_stale())
			stale.push_back(*i);
	for(list<Object *>::iterator i=stale.begin(); i!=stale.end(); ++i)
		delete *i;

	for(list<BonusEffect *>::iterator i=effects.begin(); i!=effects.end();)
	{
		if((*i)->is_stale())
		{
			delete *i;
			i=effects.erase(i);
		}
		else
			++i;
	}

	scape.tick();
}

void Playfield::render() const
{
	camera.apply();
	if(quake_strength)
		GL::translate(0, sin(quake_phase)*0.02, 0);

	if(shadow)
	{
		shadow->create(scene);
		glViewport(0, 0, flipout.get_window_width(), flipout.get_window_height());
	}

	if(glow)
	{
		glow->create(scene);
		glViewport(0, 0, flipout.get_window_width(), flipout.get_window_height());
	}

	lighting_combined.bind();

	if(reflect)
	{
		if(flipout.get_settings().use_shaders)
			reflect->create(scene, GL::Tag(), light_combined);
		else
			reflect->create(scene, "noshader", light_combined);
	}

	if(shadow)
	{
		lighting_diffuse.bind();

		GL::TexUnit::activate(2);
		shadow->apply();
		GL::TexUnit::activate(0);

		if(flipout.get_settings().use_shaders)
			scene.render("light");
		else
			scene.render("noshader");

		GL::TexUnit::activate(2);
		GL::Texture::unbind();
		GL::TexUnit::activate(0);

		GL::blend_func(GL::SRC_ALPHA, GL::ONE);

		lighting_fill.bind();
	}

	if(flipout.get_settings().use_shaders)
		scene.render();
	else
		scene.render("noshader");

	GL::Lighting::unbind();
	GL::Program::unbind();
	GL::blend_func(GL::SRC_ALPHA, GL::ONE_MINUS_SRC_ALPHA);

	if(glow)
		glow->render();

	GL::Texture::unbind();

	if(!effects.empty())
	{
		float h=18;
		float w=h*flipout.get_window_aspect();

		GL::matrix_mode(GL::PROJECTION);
		GL::load_identity();
		GL::ortho_centered(w, h);
		GL::matrix_mode(GL::MODELVIEW);
		GL::load_identity();
		GL::translate(w/2-1, h/2-2.5, -0.5);
		GL::disable(GL::DEPTH_TEST);

		lighting_combined.bind();

		for(list<BonusEffect *>::const_iterator i=effects.begin(); i!=effects.end(); ++i)
		{
			(*i)->render();

			GL::translate(0, -1.5, 0);
		}

		GL::Lighting::unbind();
		GL::enable(GL::DEPTH_TEST);
	}
}

void Playfield::near_callback(void *data, dGeomID g1, dGeomID g2)
{
	list<ContactInfo> *ct=reinterpret_cast<list<ContactInfo> *>(data);
	ct->push_back(ContactInfo(g1, g2));
}

bool Playfield::collide(ContactInfo &cinfo)
{
	if((cinfo.count=dCollide(cinfo.g1, cinfo.g2, 4, &cinfo.contacts[0].geom, sizeof(dContact))))
	{
		dSurfaceParameters &surf=cinfo.contacts[0].surface;
		surf.mode=0;
		surf.mu=0;
		surf.bounce_vel=0.03;

		cinfo.o1=reinterpret_cast<Object *>(dGeomGetData(cinfo.g1));
		cinfo.o2=reinterpret_cast<Object *>(dGeomGetData(cinfo.g2));

		bool ok=true;
		if(cinfo.o1 && cinfo.o2)
		{
			for(list<Collider *>::iterator i=colliders.begin(); i!=colliders.end(); ++i)
			{
				int res=(*i)->collide(*cinfo.o1, *cinfo.o2, cinfo.contacts[0]);
				if(res>=0)
				{
					ok=(res!=0);
					break;
				}
			}
		}

		if(ok && flipout.get_settings().optimize_physics)
		{
			if(cinfo.o1)
				cinfo.o1->contact(cinfo.o2);
			if(cinfo.o2)
				cinfo.o2->contact(cinfo.o1);
		}

		return ok;
	}

	return false;
}

void Playfield::create_contacts(ContactInfo &cinfo)
{
	for(int i=0; i<cinfo.count; ++i)
	{
		if(i>0)
			cinfo.contacts[i].surface=cinfo.contacts[0].surface;

		dBodyID b1=dGeomGetBody(cinfo.g1);
		dBodyID b2=dGeomGetBody(cinfo.g2);
		if(b1 || b2)
		{
			dJointID joint=dJointCreateContact(world, contacts, cinfo.contacts+i);
			dJointAttach(joint, b1, b2);

			++n_contacts;
		}
	}
}

void Playfield::ball_stuck(bool s)
{
	if(s)
	{
		if(!--moving_balls)
			balls_stuck=true;
	}
	else
		++moving_balls;
}

void Playfield::activate_objects()
{
	for(set<Object *>::iterator i=objects.begin(); i!=objects.end(); ++i)
		if(DynamicObject *dobj=dynamic_cast<DynamicObject *>(*i))
			dobj->activate();
}

bool Playfield::collide_ball_brick(Ball &ball, Brick &brick, dContact &contact)
{
	Vector3 vel=ball.get_linear_velocity();
	double speed=abs(vel.x*contact.geom.normal[0] + vel.y*contact.geom.normal[1] + vel.z*contact.geom.normal[2]);

	const BrickType &br_type=brick.get_type();
	contact.surface.mode|=dContactBounce;
	contact.surface.bounce=br_type.get_bouncyness();
	if(br_type.get_boost())
		contact.surface.bounce*=(speed+br_type.get_boost())/speed;

	unsigned power=ball.get_type().get_power();
	bool passthrough=(!brick.get_type().is_indestructible() && brick.get_durability()<power);
	brick.hit(power, speed);
	return !passthrough;
}

bool Playfield::collide_ball_bonus(Ball &ball, Bonus &bonus, dContact &contact)
{
	contact.surface.mode|=dContactBounce;
	contact.surface.bounce=0.7;

	if(ball.get_type().get_collect_bonuses())
	{
		bonus.collect();
		++bonuses_collected;
	}

	return true;
}

bool Playfield::collide_ball_ball(Ball &, Ball &, dContact &contact)
{
	contact.surface.mode|=dContactBounce;
	contact.surface.bounce=1;

	return true;
}

bool Playfield::collide_paddle_bonus(Paddle &, Bonus &bonus, dContact &)
{
	if(!bonus.is_stale())
	{
		bonus.collect();
		++bonuses_collected;
	}

	return false;
}

bool Playfield::collide_paddle_object(Paddle &, Object &, dContact &contact)
{
	contact.surface.mode|=dContactBounce;
	contact.surface.bounce=1;

	return true;
}

bool Playfield::collide_brick_brick(Brick &, Brick &, dContact &contact)
{
	contact.surface.mu=2*friction_modifier;

	return true;
}

bool Playfield::collide_brick_bonus(Brick &, Bonus &, dContact &contact)
{
	contact.surface.mu=2*friction_modifier;

	return true;
}

bool Playfield::collide_wall_object(Wall &wall, Object &obj, dContact &contact)
{
	if(wall.get_side()==Wall::FLOOR)
	{
		if(friction_modifier<1)
			contact.surface.mu=1/(1-friction_modifier)-1;
		else
			contact.surface.mu=dInfinity;

		if(quake_strength>0)
		{
			DynamicObject *dobj=dynamic_cast<DynamicObject *>(&obj);
			if(dobj)
			{
				float s=sin(quake_phase);
				float m=dobj->get_mass();
				Vector3 f;
				f.z=s*s*quake_strength*m*quake_scale;
				f.y=s*quake_strength*0.3*m*quake_scale;
				dobj->add_force(f);
			}
		}
	}
	else
	{
		contact.surface.mode|=dContactBounce;
		contact.surface.bounce=1;
		contact.surface.bounce_vel=0.03;
	}

	return true;
}
