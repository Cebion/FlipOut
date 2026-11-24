/* $Id: vslider.h 20 2007-12-25 20:41:08Z tdb $

This file is part of libmspgltk
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_GLTK_VSLIDER_H_
#define MSP_GLTK_VSLIDER_H_

#include "slider.h"

namespace Msp {
namespace GLtk {

class VSlider: public Slider
{
private:
	unsigned slider_size;

public:
	VSlider(const Resources &);
	virtual void button_press(int, int, unsigned);
	virtual void button_release(int, int, unsigned);
	virtual void pointer_motion(int, int);
private:
	virtual const char *get_class() const { return "vslider"; }
	virtual void render_special(const Part &) const;

	virtual void on_geometry_change();
	virtual void on_style_change();
};

} // namespace GLtk
} // namespace Msp

#endif
