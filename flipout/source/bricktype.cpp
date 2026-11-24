/* $Id$

This file is part of flipout
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include <msp/time/units.h>
#include "bricktype.h"
#include "resources.h"

using namespace std;
using namespace Msp;

BrickType::BrickType():
	width(1),
	height(1),
	depth(1),
	durability(1),
	indestructible(false),
	bouncyness(1),
	bonus_chance(1),
	boost(0),
	score(10),
	hit_sound(0)
{ }

const BrickColor &BrickType::get_color(const string &n) const
{
	return colors.get(n);
}


BrickType::Explosion::Explosion():
	type(0),
	count(10),
	speed(5),
	timeout(5*Msp::Time::sec)
{ }


BrickType::Loader::Loader(BrickType &bt, Collection &c):
	Object::Loader(bt, c),
	btype(bt)
{
	add("bonus_chance",    &BrickType::bonus_chance);
	add("boost",           &BrickType::boost);
	add("bouncyness",      &BrickType::bouncyness);
	add("color_scheme",    &Loader::color_scheme);
	add("color",           &Loader::color);
	add("durability",      &BrickType::durability);
	add("explode",         &Loader::explode);
	add("hit_sound",       &BrickType::hit_sound);
	add("indestructible",  &BrickType::indestructible);
	add("score",           &BrickType::score);
	add("size",            &Loader::size);
}

void BrickType::Loader::size(float w, float h, float d)
{
	btype.width=w;
	btype.height=h;
	btype.depth=d;
}

void BrickType::Loader::color_scheme(const string &n)
{
	btype.colors.add(*coll.get<ColorScheme>(n));
}

void BrickType::Loader::color(const string &n)
{
	BrickColor bc;
	load_sub(bc);
	btype.colors.add(n, bc);
}

void BrickType::Loader::explode(const string &n)
{
	btype.explosion.type=coll.get<BrickType>(n);
	load_sub(btype.explosion);
}


BrickType::Explosion::Loader::Loader(Explosion &e):
	expl(e)
{
	add("count",   &Explosion::count);
	add("speed",   &Explosion::speed);
	add("timeout", &Loader::timeout);
}

void BrickType::Explosion::Loader::timeout(float t)
{
	expl.timeout=t*Msp::Time::sec;
}
