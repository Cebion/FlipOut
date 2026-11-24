/* $Id: shadowmap.h 84 2009-09-21 13:50:58Z tdb $

This file is part of libmspgl
Copyright © 2009  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef SHADOWMAP_H_
#define SHADOWMAP_H_

#include "effect.h"
#include "framebuffer.h"
#include "texture2d.h"
#include "vector.h"

namespace Msp {
namespace GL {

class Light;
class Scene;

/**
Creates shadows on a Scene through a shadow map texture.  In the preparation
phase, the scene is rendered to a depth texture from the point of view of the
lightsource.  This texture is then used in the rendering phase together with
texture coordinate generation to determine whether each fragment is lit.
*/
class ShadowMap: public Effect
{
private:
	unsigned size;
	const Scene &scene;
	const Light &light;
	Framebuffer fbo;
	unsigned unit;
	Texture2D depth_buf;
	Vector3 target;
	float radius;

public:
	ShadowMap(unsigned, const Scene &, const Light &);

	/** Sets the ShadowMap target point and radius.  The transformation matrix is
	computed so that a sphere with the specified parameters will be completely
	covered by the ShadowMap. */
	void set_target(const Vector3 &, float);

	/** Sets the texture unit to bind the shadow map to during the rendering
	phase.  The default is texture unit 3. */
	void set_texture_unit(unsigned);

	virtual void prepare();
	virtual void cleanup();
};

} // namespace GL
} // namespace Msp

#endif
