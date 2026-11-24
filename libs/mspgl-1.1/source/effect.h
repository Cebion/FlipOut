/* $Id: effect.h 76 2009-01-27 19:38:22Z tdb $

This file is part of libmspgl
Copyright © 2009  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_GL_EFFECT_H_
#define MSP_GL_EFFECT_H_

namespace Msp {
namespace GL {

class Effect
{
public:
	virtual void prepare() =0;
	virtual void cleanup() =0;
};

} // namespace GL
} // namespace Msp

#endif
