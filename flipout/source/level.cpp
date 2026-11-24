/* $Id$

This file is part of flipout
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include <cmath>
#include <msp/core/refptr.h>
#include <msp/gl/immediate.h>
#include <msp/gl/matrix.h>
#include <msp/gl/misc.h>
#include <msp/gl/transform.h>
#include "bonustype.h"
#include "brick.h"
#include "bricktype.h"
#include "flipout.h"
#include "level.h"
#include "playfield.h"
#include "resources.h"

using namespace std;
using namespace Msp;

Level::Level():
	width(20),
	height(15),
	gravity(8),
	bonus_chance(0.3)
{ }

void Level::set_title(const string &t)
{
	title=t;
}

void Level::set_size(float w, float h)
{
	width=w;
	height=h;
}

void Level::add_brick(const BrickDef &b)
{
	bricks.push_back(b);
}

void Level::remove_brick(const BrickDef &b)
{
	for(list<BrickDef>::iterator i=bricks.begin(); i!=bricks.end(); ++i)
		if(&*i==&b)
		{
			bricks.erase(i);
			return;
		}
}

Playfield *Level::create_playfield(FlipOut &flipout) const
{
	RefPtr<Playfield> pf=new Playfield(flipout, width, height);
	pf->set_gravity(gravity);

	list<BonusType *> bonus_types=flipout.get_resources().get_list<BonusType>();

	for(list<BrickDef>::const_iterator i=bricks.begin(); i!=bricks.end(); ++i)
	{
		Brick *brick=new Brick(*pf, *i->type);
		brick->set_position(Vector3(i->x, i->y, i->z+0.5));
		brick->set_quaternion(Quaternion::from_axis_and_angle(Vector3(0, 0, 1), i->angle));
		brick->set_color(i->type->get_color(i->color));
		if(i->bonus)
			brick->set_bonus_type(i->bonus);
		else
		{
			float r=float(rand())/RAND_MAX;
			if(!i->type->is_indestructible() && r<bonus_chance*i->type->get_bonus_chance())
				brick->set_bonus_type(&random_bonus_type(bonus_types));
		}
	}

	return pf.release();
}

void Level::save(list<DataFile::Statement> &st, const Resources &res) const
{
	if(!title.empty())
		st.push_back((DataFile::Statement("title"), title));

	st.push_back((DataFile::Statement("size"), width, height));
	st.push_back((DataFile::Statement("gravity"), gravity));
	st.push_back((DataFile::Statement("bonus_chance"), bonus_chance));

	for(list<BrickDef>::const_iterator i=bricks.begin(); i!=bricks.end(); ++i)
	{
		DataFile::Statement ss("brick");
		ss.append(res.get_name(i->type));
		i->save(ss.sub, res);
		st.push_back(ss);
	}
}

void Level::render(const GL::Tag &pass) const
{
	GL::disable(GL_LIGHTING);
	GL::Immediate imm((GL::COLOR4_UBYTE, GL::VERTEX2));
	imm.begin(GL::QUADS);
	imm.color(0.5f, 0.5f, 0.5f, 1.0f);
	imm.vertex(-width/2, 0);
	imm.vertex(width/2, 0);
	imm.vertex(width/2, height);
	imm.vertex(-width/2, height);
	imm.end();

	GL::enable(GL_LIGHTING);

	for(list<BrickDef>::const_iterator i=bricks.begin(); i!=bricks.end(); ++i)
	{
		GL::push_matrix();
		GL::translate(i->x, i->y, i->z+i->type->get_depth()*0.5);
		GL::rotate(i->angle*180/M_PI, 0, 0, 1);

		GL::Material mat;
		mat.set_diffuse(i->type->get_color(i->color).color);
		mat.bind();

		i->type->render(pass);

		GL::Material::unbind();
		GL::pop_matrix();
	}
}

BonusType &Level::random_bonus_type(const list<BonusType *> &bonus_types) const
{
	double total_prob=0;
	for(list<BonusType *>::const_iterator i=bonus_types.begin(); i!=bonus_types.end(); ++i)
		total_prob+=(*i)->get_probability();

	double r=rand()*total_prob/RAND_MAX;
	for(list<BonusType *>::const_iterator i=bonus_types.begin(); i!=bonus_types.end(); ++i)
	{
		float p=(*i)->get_probability();
		if(r<=p)
			return **i;
		else
			r-=p;
	}

	throw Exception("Ran out of bonus types in Level::random_bonus_type");
}


Level::BrickDef::BrickDef(const BrickType *t):
	type(t),
	x(0), y(0), z(0),
	angle(0),
	bonus(0)
{ }

void Level::BrickDef::save(list<DataFile::Statement> &st, const Resources &res) const
{
	st.push_back((DataFile::Statement("color"), color));
	st.push_back((DataFile::Statement("position"), x, y));
	if(z!=0)
		st.push_back((DataFile::Statement("layer"), z));
	if(abs(angle)>1e-5)
		st.push_back((DataFile::Statement("angle"), angle*180/M_PI));
	if(bonus)
		st.push_back((DataFile::Statement("bonus"), res.get_name(bonus)));
}


Level::Loader::Loader(Level &l, Resources &r):
	level(l),
	res(r)
{
	add("bonus_chance", &Level::bonus_chance);
	add("brick",        &Loader::brick);
	add("gravity",      &Level::gravity);
	add("size",         &Level::width, &Level::height);
	add("title",        &Level::title);
}

void Level::Loader::brick(const string &type)
{
	BrickDef bdef(res.get<BrickType>(type));
	load_sub(bdef, res);
	bdef.type->get_color(bdef.color);
	level.bricks.push_back(bdef);
}


Level::BrickDef::Loader::Loader(BrickDef &bd, Resources &r):
	bdef(bd),
	res(r)
{
	add("angle",    &Loader::angle);
	add("color",    &BrickDef::color);
	add("layer",    &BrickDef::z);
	add("position", &BrickDef::x, &BrickDef::y);
	add("bonus",    &BrickDef::bonus);
}

void Level::BrickDef::Loader::angle(float a)
{
	bdef.angle=a*M_PI/180.0;
}
