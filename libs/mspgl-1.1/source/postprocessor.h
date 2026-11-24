/* $Id: postprocessor.h 76 2009-01-27 19:38:22Z tdb $

This file is part of libmspgl
Copyright © 2009  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_GL_POSTPROCESSOR_H_
#define MSP_GL_POSTPROCESSOR_H_

namespace Msp {
namespace GL {

class Texture2D;

/**
Base class for post-processing effects.
*/
class PostProcessor
{
protected:
	PostProcessor() { }
public:
	/**
	Renders the effect.  Takes the source texture as a parameter.
	*/
	virtual void render(const Texture2D &) =0;
};

} // namespace GL
} // namespace Msp

#endif
