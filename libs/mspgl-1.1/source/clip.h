/* $Id: clip.h 52 2008-07-19 11:19:01Z tdb $

This file is part of libmspgl
Copyright © 2008  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_GL_CLIP_H_
#define MSP_GL_CLIP_H_

namespace Msp {
namespace GL {

class ClipPlane
{
public:
	double eq[4];

	ClipPlane(double, double, double, double);
	void apply_to(unsigned);

	static void disable(unsigned);
};

} // namespace GL
} // namespace Msp

#endif
