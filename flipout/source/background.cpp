/* $Id$

This file is part of flipout
Copyright © 2007-2008  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include "background.h"
#include "resources.h"

Background::Background():
	shader(0),
	shdata(0),
	tex(0),
	reflect(0)
{ }

Background::~Background()
{
	delete shdata;
}

void Background::apply(bool use_shader) const
{
	tex->bind();
	if(shader && use_shader)
	{
		shader->bind();
		shdata->apply();
	}
}


Background::Loader::Loader(Background &b, Collection &c):
	bg(b),
	coll(c)
{
	add("shader",  &Background::shader);
	add("texture", &Background::tex);
	add("reflect", &Background::reflect);
}

void Background::Loader::finish()
{
	if(bg.shader)
	{
		bg.shdata=new Msp::GL::ProgramData;
		bg.shdata->uniform(bg.shader->get_uniform_location("texture"), 0);
		bg.shdata->uniform(bg.shader->get_uniform_location("refl_tex"), 1);
		bg.shdata->uniform(bg.shader->get_uniform_location("refl_str"), bg.reflect);
	}
}
