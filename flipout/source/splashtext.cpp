/* $Id$

This file is part of flipout
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include <msp/gl/immediate.h>
#include <msp/gl/transform.h>
#include <msp/time/units.h>
#include "splashtext.h"

using namespace std;
using namespace Msp;

SplashText::SplashText(const string &t, const GL::Font &f):
	font(f),
	text(t),
	zoom(0)
{ }

void SplashText::set_color(const GL::Color &c)
{
	color=c;
}

void SplashText::tick(const Time::TimeDelta &dt)
{
	zoom+=dt/Time::sec*2;
}

void SplashText::render() const
{
	float text_w=font.get_string_width(text);

	GL::translate(-text_w/2, -0.5, 0);

	GL::Immediate imm((GL::COLOR4_UBYTE, GL::TEXCOORD2, GL::VERTEX2));
	imm.color(color.r, color.g, color.b, color.a);
	font.draw_string(text, imm);

	if(zoom<1)
	{
		GL::translate(-text_w*0.2*zoom, -0.2*zoom, 0);
		GL::scale_uniform(1.0+0.4*zoom);
		imm.color(color.r, color.g, color.b, color.a*(1-zoom));
		font.draw_string(text, imm);
	}
}
