/* $Id$

This file is part of flipout
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include <msp/gl/blend.h>
#include <msp/gl/misc.h>
#include <msp/gl/program.h>
#include <msp/gl/tests.h>
#include <msp/gl/texunit.h>
#include "flipout.h"
#include "glow.h"
#include "misc.h"

using namespace Msp;

Glow::Glow(FlipOut &flipout):
	width(flipout.get_window_width()/2),
	height(flipout.get_window_height()/2),
	framebuf(new GL::Framebuffer),
	depthbuf(new GL::Renderbuffer),
	shader(flipout.get_resources().get<GL::Program>("glow"))
{
	depthbuf->storage(GL::DEPTH_COMPONENT, width, height);
	framebuf->attach(GL::DEPTH_ATTACHMENT, *depthbuf);

	float ox=1.0f/width;
	float oy=1.0f/height;

	for(unsigned i=0; i<2; ++i)
	{
		tex[i]=new GL::Texture2D;
		tex[i]->set_min_filter(GL::NEAREST);
		tex[i]->storage(GL::RGB, width, height, 0);
		tex[i]->image(0, GL::RGB, GL::UNSIGNED_BYTE, 0);

		shdata[i]=new GL::ProgramData;
		shdata[i]->uniform(shader->get_uniform_location("texture1"), 0);
		shdata[i]->uniform(shader->get_uniform_location("texture2"), 1);
	}

	shdata[0]->uniform(shader->get_uniform_location("offset"), ox, 0);
	shdata[1]->uniform(shader->get_uniform_location("offset"), 0, oy);

	GL::Framebuffer::unbind();
}

Glow::~Glow()
{
	delete framebuf;
	delete depthbuf;
	for(unsigned i=0; i<2; ++i)
	{
		delete tex[i];
		delete shdata[i];
	}
}

void Glow::create(const GL::Scene &scene)
{
	GL::enable(GL_LIGHTING);
	float black[4]={0.0, 0.0, 0.0, 1.0};
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, black);
	GL::disable(GL_LIGHT0);
	framebuf->bind();
	glViewport(0, 0, width, height);
	framebuf->attach(GL::COLOR_ATTACHMENT0, *tex[0], 0);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	scene.render("glow");

	GL::disable(GL::DEPTH_TEST);
	shader->bind();

	for(unsigned i=0; i<4; ++i)
	{
		framebuf->attach(GL::COLOR_ATTACHMENT0, *tex[1-i%2], 0);

		shdata[i%2]->apply();
		for(unsigned j=0; j<2; ++j)
		{
			GL::TexUnit::activate(j);
			tex[i%2]->bind();
		}
		draw_fullscreen_quad();
	}

	GL::Program::unbind();
	GL::Framebuffer::unbind();
	GL::disable(GL_LIGHTING);
	GL::enable(GL::DEPTH_TEST);
	GL::Texture::unbind();
	GL::TexUnit::activate(0);
	GL::Texture::unbind();
}

void Glow::render() const
{
	GL::blend_func(GL::ONE, GL::ONE);
	GL::disable(GL::DEPTH_TEST);

	tex[0]->bind();
	draw_fullscreen_quad();

	GL::blend_func(GL::SRC_ALPHA, GL::ONE_MINUS_SRC_ALPHA);
	GL::enable(GL::DEPTH_TEST);
}
