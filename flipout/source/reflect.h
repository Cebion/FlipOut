/* $Id$

This file is part of flipout
Copyright © 2007-2008  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef REFLECT_H_
#define REFLECT_H_

#include <msp/gl/framebuffer.h>
#include <msp/gl/light.h>
#include <msp/gl/scene.h>

class FlipOut;

class Reflect
{
private:
	Msp::GL::Framebuffer *framebuf;
	Msp::GL::Renderbuffer *depthbuf;
	Msp::GL::Texture2D *tex;

public:
	Reflect(FlipOut &);
	~Reflect();

	void create(const Msp::GL::Scene &, const Msp::GL::Tag &, const Msp::GL::Light &);
	const Msp::GL::Texture2D *get_texture() const { return tex; }
};

#endif
