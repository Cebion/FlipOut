/* $Id: indicator.cpp 27 2008-03-03 17:48:55Z tdb $

This file is part of libmspgltk
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include "indicator.h"

namespace Msp {
namespace GLtk {

Indicator::Indicator(const Resources &r):
	Widget(r)
{
	update_style();
}

void Indicator::set_active(bool a)
{
	if(a)
		state|=ACTIVE;
	else
		state&=~ACTIVE;
}

} // namespace GLtk
} // namespace Msp
