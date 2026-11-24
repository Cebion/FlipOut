/* $Id$

This file is part of flipout
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include <msp/al/source.h>
#include <msp/gl/matrix.h>
#include <msp/gl/objectpass.h>
#include <msp/gl/program.h>
#include <msp/gl/programdata.h>
#include <msp/gl/technique.h>
#include <msp/gl/transform.h>
#include <msp/time/units.h>
#include "ball.h"
#include "bonus.h"
#include "brick.h"
#include "bricktype.h"
#include "flipout.h"
#include "geomcategory.h"
#include "misc.h"
#include "playfield.h"

using namespace std;
using namespace Msp;

Brick::Brick(Playfield &pf, const BrickType &t):
	DynamicObject(pf),
	GL::ObjectInstance(t),
	type(t),
	durability(type.get_durability()),
	bonus_type(0),
	shdata(0)
{
	geom=dCreateBox(playfield.get_space(), type.get_width()-0.02, type.get_height()-0.02, type.get_depth()-0.02);

	dMass mass;
	dMassSetBox(&mass, 1, type.get_width(), type.get_height(), type.get_depth());
	dBodySetMass(body, &mass);

	dGeomSetBody(geom, body);
	dGeomSetCategoryBits(geom, BRICK);
	dGeomSetCollideBits(geom, FLOOR|WALL|BRICK);
	dGeomSetData(geom, static_cast<Object *>(this));

	set_color(type.get_color());

	if(GL::Program *shader=type.get_technique()->get_pass(GL::Tag()).shprog)
	{
		shdata=new GL::ProgramData;
		float random[4];
		for(unsigned i=0; i<4; ++i)
			random[i]=float(rand())/RAND_MAX;
		shdata->uniform4(shader->get_uniform_location("random"), random);
	}

	playfield.add(*this);
	playfield.get_scene().add(*this);
}

Brick::~Brick()
{
	delete shdata;

	playfield.remove(*this);
	playfield.get_scene().remove(*this);
}

void Brick::set_color(const BrickColor &color)
{
	set_color(color.random());
}

void Brick::set_color(const GL::Color &color)
{
	material.set_diffuse(color);
	material.set_ambient(color);
}

void Brick::set_timeout(const Msp::Time::TimeDelta &to)
{
	timeout=to;
}

void Brick::hit(unsigned power, float speed)
{
	Vector3 pos=get_position();

	if(playfield.get_flipout().get_settings().sfx_enabled && speed>0.5)
	{
		if(const AL::Buffer *snd=type.get_hit_sound())
		{
			AL::Source &src=playfield.get_soundscape().play(*snd, pos.x, pos.y, pos.z);
			src.set_rolloff_factor(0);
			src.set_gain(playfield.get_flipout().get_settings().sfx_volume*min(speed*0.2f, 1.0f)/100.0f);
		}
	}

	if(type.is_indestructible() || durability==0 || (speed<1 && power<=durability))
		return;

	if(durability<=power)
	{
		durability=0;
		stale=true;
		playfield.signal_score.emit(type.get_score());

		const BonusType *bt=bonus_type;
		if(!bt && type.get_bonus_chance())
			bt=playfield.get_fill_bonus();

		if(bt)
		{
			Bonus *bonus=new Bonus(playfield, *bt);
			bonus->set_position(get_position());
			bonus->set_quaternion(get_quaternion());
			//Quaternion::from_axis_and_angle(Vector3(0, 0, 1), double(rand())/RAND_MAX*0.4-0.2));
		}

		const BrickType::Explosion &explosion=type.get_explosion();
		if(explosion.type && playfield.get_flipout().get_settings().explosions)
		{
			float ew=explosion.type->get_width();
			float eh=explosion.type->get_height();
			float ed=explosion.type->get_depth();
			float aw=type.get_width()-ew;
			float ah=type.get_height()-eh;
			float ad=type.get_depth()-ed;

			list<Vector3> points;
			for(unsigned i=0; i<explosion.count; ++i)
				points.push_back(Vector3(aw*(1.0*rand()/RAND_MAX-0.5), ah*(1.0*rand()/RAND_MAX-0.5), ad*(1.0*rand()/RAND_MAX-0.5)));

			float spacing=2*pow(ew*eh*ed*3/(4*M_PI), 0.333);

			/* Move the points apart from each other.  This reduces initial
			constraint violations and makes the simulation behave better with a
			small timestep. */
			for(unsigned i=0; i<10; ++i)
			{
				unsigned n=0;
				for(list<Vector3>::iterator j=points.begin(); j!=points.end(); ++j)
					for(list<Vector3>::iterator k=j; ++k!=points.end();)
					{
						Vector3 dist=*k-*j;
						if(dot(dist, dist)<spacing*spacing)
						{
							*j=*j-dist*0.5;
							*k=*k+dist*0.5;
							++n;
						}
					}

				for(list<Vector3>::iterator j=points.begin(); j!=points.end(); ++j)
				{
					j->x=min(max(j->x, -aw*0.5), aw*0.5);
					j->y=min(max(j->y, -ah*0.5), ah*0.5);
					j->z=min(max(j->z, -ad*0.5), ad*0.5);
				}
			}

			const dReal *rot=dBodyGetRotation(body);

			for(list<Vector3>::iterator i=points.begin(); i!=points.end(); ++i)
			{
				Vector3 p(pos.x+i->x*rot[0]+i->y*rot[1]+i->z*rot[2],
					pos.y+i->x*rot[4]+i->y*rot[5]+i->z*rot[6],
					pos.z+i->x*rot[8]+i->y*rot[9]+i->z*rot[10]);

				Brick *piece=new Brick(playfield, *explosion.type);
				piece->set_position(p);
				piece->set_color(material.get_diffuse());

				float s=explosion.speed;
				Vector3 lv((p.x-pos.x)*s, (p.y-pos.y)*s, (p.z-pos.z+ad/2)*s);
				piece->set_linear_velocity(lv);

				Vector3 av(1.0*rand()/RAND_MAX-0.5, 1.0*rand()/RAND_MAX-0.5, 1.0*rand()/RAND_MAX-0.5);
				piece->set_angular_velocity(av);
				piece->set_quaternion(Quaternion::from_axis_and_angle(av, 6.28*rand()/RAND_MAX));

				piece->set_timeout((0.5+0.5*rand()/RAND_MAX)*explosion.timeout);
			}
		}
	}
	else
		durability-=power;
}

void Brick::step(const Msp::Time::TimeDelta &dt)
{
	DynamicObject::step(dt);

	dMass mass;
	dBodyGetMass(body, &mass);

	const dReal *avel=dBodyGetAngularVel(body);
	double f=-mass.mass*sqrt(avel[0]*avel[0] + avel[1]*avel[1] + avel[2]*avel[2])*0.005;
	dBodyAddTorque(body, avel[0]*f, avel[1]*f, avel[2]*f);

	if(timeout)
	{
		timeout-=dt;
		if(timeout<=Msp::Time::zero)
			stale=true;
	}
}

void Brick::setup_render(const GL::Tag &tag) const
{
	GL::push_matrix();

	double matrix[16];
	get_matrix(body, matrix);
	GL::mult_matrix(matrix);

	if(type.get_technique()->get_pass(tag).shprog && shdata)
		shdata->apply();

	material.bind();

	// For debugging physics optimization
	/*float color[4]={0.2, 0.2, 0.2, 1.0};
	if(dBodyIsEnabled(body))
		color[0]=0.7;
	if(moving)
		color[1]=0.7;
	color[2]=min(inactive_count/10., 1.0);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, color);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, color);*/
}

void Brick::finish_render(const GL::Tag &) const
{
	GL::Material::unbind();
	GL::pop_matrix();
}

unsigned Brick::get_level_of_detail() const
{
	return playfield.get_flipout().get_settings().simple_models ? 1 : 0;
}
