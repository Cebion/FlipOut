/* $Id: hslider.cpp 46 2008-10-11 15:09:51Z tdb $

This file is part of libmspgltk
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include <msp/gl/matrix.h>
#include <msp/gl/transform.h>
#include "graphic.h"
#include "hslider.h"
#include "part.h"
#include "style.h"

namespace Msp {
namespace GLtk {

HSlider::HSlider(const Resources &r):
	Slider(r)
{
	update_style();
}

void HSlider::button_press(int x, int y, unsigned btn)
{
	if(btn==1 && geom.is_inside_relative(x, y) && max>min)
	{
		int sx=static_cast<int>((geom.w-slider_size)*(value-min)/(max-min));
		if(x<sx)
			set_value(value-step*10);
		else if(x>=static_cast<int>(sx+slider_size))
			set_value(value+step*10);
		else
			start_drag(x);
	}
}

void HSlider::button_release(int, int, unsigned btn)
{
	if(btn==1 && dragging)
		end_drag();
}

void HSlider::pointer_motion(int x, int)
{
	if(dragging)
		drag(x);
}

void HSlider::render_special(const Part &part) const
{
	if(part.get_name()=="slider")
	{
		Alignment align=part.get_alignment();
		if(max>min)
			align.x=(value-min)/(max-min);

		Geometry pgeom=part.get_geometry();
		align.apply(pgeom, geom, part.get_margin());

		GL::push_matrix();
		GL::translate(pgeom.x, pgeom.y, 0);
		part.get_graphic(state)->render(pgeom.w, pgeom.h);
		GL::pop_matrix();
	}
}

void HSlider::on_geometry_change()
{
	drag_area_size=geom.w-slider_size;
}

void HSlider::on_style_change()
{
	for(PartSeq::const_iterator i=style->get_parts().begin(); i!=style->get_parts().end(); ++i)
		if(i->get_name()=="slider")
			slider_size=i->get_geometry().w;

	on_geometry_change();
}

} // namespace GLtk
} // namespace Msp
