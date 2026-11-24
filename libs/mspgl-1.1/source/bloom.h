/* $Id: bloom.h 76 2009-01-27 19:38:22Z tdb $

This file is part of libmspgl
Copyright © 2009  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_GL_BLOOM_H_
#define MSP_GL_BLOOM_H_

#include "framebuffer.h"
#include "mesh.h"
#include "postprocessor.h"
#include "texture2d.h"
#include "program.h"
#include "programdata.h"

namespace Msp {
namespace GL {

/**
The Bloom post-processing effect causes very bright areas of the image to bleed
into surrounding pixels.  Commonly used together with HDR rendering.

The technique used is to gaussian blur the image and then blend the result with
the original image.  With suitable parameters, this effect may also be used as
a blur filter.
*/
class Bloom: public PostProcessor
{
private:
	Framebuffer fbo;
	Texture2D tex[2];
	Program blur_shader;
	ProgramData blur_shdata[2];
	Program combine_shader;
	ProgramData combine_shdata;
	Mesh quad;

public:
	Bloom(unsigned, unsigned);
	
	/** Sets the σ value of the gaussian blur.  Values much larger than 4.0 are
	likely to cause artifacts. */
	void set_radius(float);

	/** Sets the blend factor between original and blurred images.  Larger
	values mean more blurriness. */
	void set_strength(float);

	virtual void render(const Texture2D &);
};

} // namespace GL
} // namespace Msp

#endif
