/* $Id: slider.h 20 2007-12-25 20:41:08Z tdb $

This file is part of libmspgltk
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_GLTK_SLIDER_H_
#define MSP_GLTK_SLIDER_H_

#include <sigc++/sigc++.h>
#include "widget.h"

namespace Msp {
namespace GLtk {

/**
Sliders are used to adjust numeric values visually.  This class provides the
common interface for sliders - see classes HSlider and VSlider for concrete
variations.
*/
class Slider: public Widget
{
public:
	class Loader: public Widget::Loader
	{
	public:
		Loader(Slider &);
		Slider &get_object() const;
	};

protected:
	double min, max;
	double value;
	double step;

	bool dragging;
	double drag_start_pos;
	double drag_start_value;
	unsigned drag_area_size;

public:
	sigc::signal<void, double> signal_value_changed;

protected:
	Slider(const Resources &);
public:
	void set_value(double);
	void set_range(double, double);
	void set_step(double);
	double get_value() const { return value; }
protected:
	void start_drag(int);
	void drag(int);
	void end_drag();
};

} // namespace GLtk
} // namespace Msp

#endif
