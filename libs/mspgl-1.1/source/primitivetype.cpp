/* $Id: primitivetype.cpp 13 2007-09-04 09:59:14Z tdb $

This file is part of libmspgl
Copyright © 2007 Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include "primitivetype.h"

namespace Msp {
namespace GL {

std::istream &operator>>(std::istream &in, PrimitiveType &pt)
{
	std::string str;
	in>>str;

	if(str=="POINTS")
		pt=POINTS;
	else if(str=="LINES")
		pt=LINES;
	else if(str=="LINE_LOOP")
		pt=LINE_LOOP;
	else if(str=="LINE_STRIP")
		pt=LINE_STRIP;
	else if(str=="TRIANGLES")
		pt=TRIANGLES;
	else if(str=="TRIANGLE_STRIP")
		pt=TRIANGLE_STRIP;
	else if(str=="TRIANGLE_FAN")
		pt=TRIANGLE_FAN;
	else if(str=="QUADS")
		pt=QUADS;
	else if(str=="QUAD_STRIP")
		pt=QUAD_STRIP;
	else if(str=="POLYGON")
		pt=POLYGON;
	else
		in.setstate(std::ios_base::failbit);

	return in;
}

} // namespace GL
} // namespace Msp
