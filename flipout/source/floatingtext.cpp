/* $Id$

This file is part of flipout
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include <msp/gl/matrix.h>
#include <msp/gl/misc.h>
#include <msp/gl/meshbuilder.h>
#include <msp/gl/tests.h>
#include <msp/gl/texture2d.h>
#include <msp/gl/transform.h>
#include <msp/time/units.h>
#include "floatingtext.h"
#include "playfield.h"

using namespace std;
using namespace Msp;

FloatingText::FloatingText(Playfield &pf, const GL::Font &font, const string &text, const GL::Color &color):
	Object(pf),
	mesh((GL::COLOR4_UBYTE, GL::TEXCOORD2, GL::VERTEX2)),
	tex(font.get_texture()),
	width(font.get_string_width(text))
{
	GL::MeshBuilder bld(mesh);
	bld.color(color.r, color.g, color.b, color.a);
	font.draw_string(text, bld);

	playfield.add(*this);
	playfield.get_scene().add(*this);
}

FloatingText::~FloatingText()
{
	playfield.remove(*this);
	playfield.get_scene().remove(*this);
}

void FloatingText::set_position(const Vector3 &p)
{
	pos=p;
}

void FloatingText::step(const Time::TimeDelta &dt)
{
	pos.z+=dt/Time::sec;
	if(pos.z>3)
		stale=true;
}

bool FloatingText::has_pass(const GL::Tag &tag) const
{
	return tag.id==0 || tag.id==GL::Tag("noshader").id || tag.id==GL::Tag("glow").id;
}

void FloatingText::render(const GL::Tag &) const
{
	GL::push_matrix();

	tex.bind();
	GL::disable(GL_LIGHTING);
	GL::disable(GL::DEPTH_TEST);

	GL::translate(pos.x-width/2, pos.y, pos.z);
	playfield.get_camera().unrotate();
	mesh.draw();

	GL::Texture::unbind();
	GL::enable(GL_LIGHTING);
	GL::enable(GL::DEPTH_TEST);

	GL::pop_matrix();
}
