/* $Id: root.cpp 55 2009-09-20 16:27:08Z tdb $

This file is part of libmspgltk
Copyright © 2007-2009  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include <msp/input/keys.h>
#include <msp/time/units.h>
#include <msp/time/utils.h>
#include "label.h"
#include "style.h"
#include "root.h"

namespace Msp {
namespace GLtk {

Root::Root(const Resources &r, Graphics::Window &w):
	Widget(r),
	Panel(r),
	window(w),
	lbl_tooltip(0),
	tooltip_target(0)
{
	set_geometry(Geometry(0, 0, window.get_width(), window.get_height()));

	update_style();

	window.signal_button_press.connect(sigc::mem_fun(this, &Root::button_press_event));
	window.signal_button_release.connect(sigc::mem_fun(this, &Root::button_release_event));
	window.signal_pointer_motion.connect(sigc::mem_fun(this, &Root::pointer_motion_event));
	window.signal_key_press.connect(sigc::mem_fun(this, &Root::key_press_event));
	window.signal_key_release.connect(sigc::mem_fun(this, &Root::key_release_event));
}

void Root::tick()
{
	if(tooltip_timeout && Time::now()>tooltip_timeout)
	{
		if(Widget *wdg=get_descendant_at(pointer_x, pointer_y))
		{
			const std::string &tip=wdg->get_tooltip();
			if(!tip.empty())
			{
				if(!lbl_tooltip)
				{
					lbl_tooltip=new Label(res);
					add(*lbl_tooltip);
					lbl_tooltip->set_style("tooltip");
				}

				lbl_tooltip->set_text(tip);
				lbl_tooltip->autosize();
				const Geometry &tip_geom=lbl_tooltip->get_geometry();
				unsigned x=pointer_x+10;
				unsigned y=pointer_y-10-lbl_tooltip->get_geometry().h;
				if(x+tip_geom.w>geom.w)
				{
					if(pointer_x>static_cast<int>(tip_geom.w+2))
						x=pointer_x-2-tip_geom.w;
					else
						x=geom.w-tip_geom.w;
				}
				lbl_tooltip->set_position(x, y);
				raise(*lbl_tooltip);
				lbl_tooltip->set_visible(true);
				tooltip_timeout=Time::TimeStamp();
				tooltip_target=wdg;
			}
		}
	}
}

void Root::button_press_event(int x, int y, unsigned btn, unsigned)
{
	if(visible)
	{
		translate_coords(x, y);
		button_press(x, y, btn);
	}
}

void Root::button_release_event(int x, int y, unsigned btn, unsigned)
{
	if(visible)
	{
		translate_coords(x, y);
		button_release(x, y, btn);
	}
}

void Root::pointer_motion_event(int x, int y)
{
	if(visible)
	{
		translate_coords(x, y);
		pointer_motion(x, y);

		if(!tooltip_target)
		{
			if(pointer_focus)
			{
				pointer_x=x;
				pointer_y=y;
				tooltip_timeout=Time::now()+700*Time::msec;
			}
			else
				tooltip_timeout=Time::TimeStamp();
		}
		else if(get_descendant_at(x, y)!=tooltip_target)
		{
			lbl_tooltip->set_visible(false);
			tooltip_target=0;
		}
	}
}

void Root::key_press_event(unsigned key, unsigned mod, wchar_t ch)
{
	if(visible)
		key_press(Input::key_from_sys(key), mod, ch);
}

void Root::key_release_event(unsigned key, unsigned mod)
{
	if(visible)
		key_release(Input::key_from_sys(key), mod);
}

void Root::translate_coords(int &x, int &y)
{
	x=x*geom.w/window.get_width();
	y=geom.h-1-y*geom.h/window.get_height();
}

} // namespace GLtk
} // namespace Msp
