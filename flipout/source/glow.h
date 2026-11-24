/* $Id$

This file is part of flipout
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef GLOW_H_
#define GLOW_H_

#include <msp/gl/framebuffer.h>
#include <msp/gl/programdata.h>
#include <msp/gl/renderbuffer.h>
#include <msp/gl/scene.h>

class FlipOut;

class Glow
{
private:
	unsigned width;
	unsigned height;
	Msp::GL::Framebuffer *framebuf;
	Msp::GL::Renderbuffer *depthbuf;
	Msp::GL::Texture2D *tex[2];
	Msp::GL::Program *shader;
	Msp::GL::ProgramData *shdata[2];

public:
	Glow(FlipOut &);
	~Glow();

	void create(const Msp::GL::Scene &);
	void render() const;
};

#endif
