/* $Id$

This file is part of flipout
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef PADDLETYPE_H_
#define PADDLETYPE_H_

#include <msp/gl/object.h>

class Resources;

class PaddleType: public Msp::GL::Object
{
private:
	float width;
	float height;
	float depth;
	bool round_ends;

public:
	class Loader: public Msp::GL::Object::Loader
	{
	private:
		PaddleType &ptype;

	public:
		Loader(PaddleType &, Collection &);
		PaddleType &get_object() const { return ptype; }
	private:
		void size(float, float, float);
	};

	PaddleType();
	float get_width() const { return width; }
	float get_height() const { return height; }
	float get_depth() const { return depth; }
	bool get_round_ends() const { return round_ends; }
};

#endif
