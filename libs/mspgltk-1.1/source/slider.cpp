/* $Id: slider.cpp 27 2008-03-03 17:48:55Z tdb $

This file is part of libmspgltk
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include "slider.h"

namespace Msp {
namespace GLtk {

Slider::Slider(const Resources &r):
	Widget(r),
	min(0),
	max(1),
	value(0),
	step(0.1),
	dragging(false),
	drag_area_size(0)
{ }

void Slider::set_value(double v)
{
	double old_value=value;

	if(v<min)
		value=min;
	else if(v>max)
		value=max;
	else
	{
		unsigned steps=static_cast<unsigned>((v-min)/step+0.5);
		value=min+steps*step;
	}

	if(value!=old_value)
		signal_value_changed.emit(value);
}

void Slider::set_range(double a, double b)
{
	min=a;
	max=b;
	set_value(value);
}

void Slider::set_step(double s)
{
	step=s;
	set_value(value);
}

void Slider::start_drag(int p)
{
	dragging=true;
	drag_start_pos=p;
	drag_start_value=value;
	state|=ACTIVE;
}

void Slider::drag(int p)
{
	if(max>min)
		set_value(drag_start_value+(p-drag_start_pos)*(max-min)/drag_area_size);
}

void Slider::end_drag()
{
	dragging=false;
	state&=~ACTIVE;
}


Slider::Loader::Loader(Slider &s):
	Widget::Loader(s)
{
	add("range", &Slider::min, &Slider::max);
	add("step",  &Slider::step);
	add("value", &Slider::value);
}

Slider &Slider::Loader::get_object() const
{
	return static_cast<Slider &>(wdg);
}

} // namespace GLtk
} // namespace Msp
