/* $Id: rendermode.cpp 8 2007-09-03 10:33:27Z tdb $

This file is part of libmspgl
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include "rendermode.h"
#include "select.h"

namespace Msp {
namespace GL {

int render_mode(RenderMode rm)
{
	int old_rm;
	glGetIntegerv(GL_RENDER_MODE, &old_rm);
	
	int result=glRenderMode(rm);
	
	if(old_rm==SELECT)
		_parse_internal_select_records(result);
	
	return result;
}

} // namespace GL
} // namespace Msp
