/* $Id$

This file is part of flipout
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef SHADOW_H_
#define SHADOW_H_

#include <msp/gl/framebuffer.h>
#include <msp/gl/programdata.h>
#include <msp/gl/scene.h>
#include "vector3.h"

class FlipOut;

class Shadow
{
private:
	unsigned size;
	Vector3 dir;
	float area;
	float min_depth;
	float max_depth;
	float depth_range;
	Msp::GL::Framebuffer *framebuf;
	Msp::GL::Texture2D *tex;

public:
	Shadow(FlipOut &, unsigned, const Vector3 &, float, float, float);
	~Shadow();

	void create(const Msp::GL::Scene &);
	void apply();
	const Msp::GL::Texture2D *get_texture() const { return tex; }
};

#endif
