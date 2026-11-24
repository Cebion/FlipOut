/* $Id: pipelinepass.h 80 2009-07-05 20:32:00Z tdb $

This file is part of libmspgl
Copyright © 2009  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_GL_PIPELINEPASS_H_
#define MSP_GL_PIPELINEPASS_H_

#include <vector>

namespace Msp {
namespace GL {

class Effect;
class Lighting;

struct PipelinePass
{
	std::vector<Effect *> effects;
	const Lighting *lighting;

	PipelinePass();
};

} // namespace GL
} // namespace Msp

#endif
