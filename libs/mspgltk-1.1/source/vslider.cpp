/* $Id: vslider.cpp 46 2008-10-11 15:09:51Z tdb $

This file is part of libmspgltk
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include <msp/gl/matrix.h>
#include <msp/gl/transform.h>
#include "graphic.h"
#include "part.h"
#include "style.h"
#include "vslider.h"

namespace Msp {
namespace GLtk {

VSlider::VSlider(const Resources &r):
	Slider(r)
{
	update_style();
}

void VSlider::button_press(int x, int y, unsigned btn)
{
	if(btn==1 && geom.is_inside_relative(x, y) && max>min)
	{
		int sy=static_cast<int>((geom.h-slider_size)*(value-min)/(max-min));
		if(y<sy)
			set_value(value-step*10);
		else if(y>=static_cast<int>(sy+slider_size))
			set_value(value+step*10);
		else
			start_drag(y);
	}
}

void VSlider::button_release(int, int, unsigned btn)
{
	if(btn==1 && dragging)
		end_drag();
}

void VSlider::pointer_motion(int, int y)
{
	if(dragging)
		drag(y);
}

void VSlider::render_special(const Part &part) const
{
	if(part.get_name()=="slider")
	{
		Alignment align=part.get_alignment();
		if(max>min)
			align.y=(value-min)/(max-min);

		Geometry pgeom=part.get_geometry();
		align.apply(pgeom, geom, part.get_margin());

		GL::push_matrix();
		GL::translate(pgeom.x, pgeom.y, 0);
		part.get_graphic(state)->render(pgeom.w, pgeom.h);
		GL::pop_matrix();
	}
}

void VSlider::on_geometry_change()
{
	drag_area_size=geom.h-slider_size;
}

void VSlider::on_style_change()
{
	for(PartSeq::const_iterator i=style->get_parts().begin(); i!=style->get_parts().end(); ++i)
		if(i->get_name()=="slider")
			slider_size=i->get_geometry().h;

	on_geometry_change();
}

} // namespace GLtk
} // namespace Msp

