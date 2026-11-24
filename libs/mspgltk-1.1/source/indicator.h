/* $Id: indicator.h 15 2007-11-27 10:59:42Z tdb $

This file is part of libmspgltk
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_GLTK_INDICATOR_H_
#define MSP_GLTK_INDICATOR_H_

#include "widget.h"

namespace Msp {
namespace GLtk {

/**
An Indicator visualizes a boolean state.  It can be either active or inactive.
*/
class Indicator: public Widget
{
public:
	Indicator(const Resources &);
	void set_active(bool);
private:
	virtual const char *get_class() const { return "indicator"; }
};

} // namespace GLtk
} // namespace Msp

#endif
