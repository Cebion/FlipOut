/* $Id$

This file is part of flipout
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef BALLTYPE_H_
#define BALLTYPE_H_

#include <msp/datafile/loader.h>
#include <msp/gl/object.h>

class Resources;

class BallType: public Msp::GL::Object
{
public:
	class Loader: public Msp::GL::Object::Loader
	{
	private:
		BallType &btype;

	public:
		Loader(BallType &, Collection &);
		BallType &get_object() const { return btype; }
	};

private:
	float radius;
	unsigned power;
	bool collect_bonuses;
	float link_distance;

public:
	BallType();
	float get_radius() const { return radius; }
	unsigned get_power() const { return power; }
	bool get_collect_bonuses() const { return collect_bonuses; }
	float get_link_distance() const { return link_distance; }
};

#endif
