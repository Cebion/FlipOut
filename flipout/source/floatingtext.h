/* $Id$

This file is part of flipout
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef FLOATINGTEXT_H_
#define FLOATINGTEXT_H_

#include <string>
#include <msp/gl/color.h>
#include <msp/gl/font.h>
#include <msp/gl/mesh.h>
#include <msp/gl/renderable.h>
#include "object.h"
#include "vector3.h"

class FloatingText: public Object, public Msp::GL::Renderable
{
private:
	Msp::GL::Mesh mesh;
	const Msp::GL::Texture2D &tex;
	Vector3 pos;
	float width;

public:
	FloatingText(Playfield &, const Msp::GL::Font &, const std::string &, const Msp::GL::Color &);
	~FloatingText();

	void set_position(const Vector3 &);
	virtual void step(const Msp::Time::TimeDelta &);
	virtual int get_order() const { return 1; }
	virtual bool has_pass(const Msp::GL::Tag &) const;
	virtual void render(const Msp::GL::Tag &) const;
};

#endif
