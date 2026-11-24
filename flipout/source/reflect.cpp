/* $Id$

This file is part of flipout
Copyright © 2007-2008  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include <msp/gl/matrix.h>
#include <msp/gl/renderbuffer.h>
#include <msp/gl/transform.h>
#include "flipout.h"
#include "reflect.h"

using namespace Msp;

Reflect::Reflect(FlipOut &flipout):
	framebuf(new GL::Framebuffer),
	depthbuf(new GL::Renderbuffer),
	tex(new GL::Texture2D)
{
	const Graphics::Window &wnd=flipout.get_window();
	depthbuf->storage(GL::DEPTH_COMPONENT, wnd.get_width(), wnd.get_height());
	framebuf->attach(GL::DEPTH_ATTACHMENT, *depthbuf);

	tex->set_min_filter(GL::NEAREST);
	tex->storage(GL::RGB, wnd.get_width(), wnd.get_height(), 0);
	tex->image(0, GL::RGB, GL::UNSIGNED_BYTE, 0);

	framebuf->attach(GL::COLOR_ATTACHMENT0, *tex, 0);

	GL::Framebuffer::unbind();
}

Reflect::~Reflect()
{
	delete framebuf;
	delete depthbuf;
	delete tex;
}

void Reflect::create(const GL::Scene &scene, const GL::Tag &pass, const GL::Light &light)
{
	GL::push_matrix();
	GL::scale(1.0, 1.0, -1.0);
	glFrontFace(GL_CW);

	// Force rebinding the light due to matrix change
	GL::Light::unbind();
	light.bind();

	framebuf->bind();
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	scene.render(pass);

	GL::Framebuffer::unbind();

	glFrontFace(GL_CCW);
	GL::pop_matrix();

	GL::Light::unbind();
	light.bind();
}
