/* $Id: widget.h 55 2009-09-20 16:27:08Z tdb $

This file is part of libmspgltk
Copyright © 2007-2009  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_GLTK_WIDGET_H_
#define MSP_GLTK_WIDGET_H_

#include <string>
#include "geometry.h"
#include "state.h"

namespace Msp {
namespace GLtk {

class Container;
class Part;
class Resources;
class Style;

/**
Base class for all widgets.  Derived classes should call update_style in 
constructor, because it can't be done correctly in the Widget constructor.
*/
class Widget
{
public:
	class Loader: public Msp::DataFile::Loader
	{
	protected:
		Widget &wdg;

	public:
		Loader(Widget &);
		Widget &get_object() const { return wdg; }
	private:
		void position(int, int);
		void size(unsigned, unsigned);
		void style(const std::string &);
	};

	sigc::signal<void, bool> signal_visibility_changed;
	sigc::signal<void> signal_request_focus;
	sigc::signal<void> signal_grab_pointer;
	sigc::signal<void> signal_ungrab_pointer;
	sigc::signal<void> signal_request_raise;

protected:
	const Resources &res;
	Geometry geom;
	std::string style_name;
	const Style *style;
	State state;
	bool visible;
	Container *parent;
	std::string tooltip;

	Widget(const Resources &);
public:
	virtual ~Widget();

	void set_position(int, int);
	void set_size(unsigned, unsigned);
	virtual void autosize() { }
	void set_geometry(const Geometry &);
	const Geometry &get_geometry() const { return geom; }

	/**
	Sets the widget style.  The final style name is constructed by concatenating
	the widget class and the style name with a dash.
	*/
	void set_style(const std::string &);
	const Style &get_style() const { return *style; }

	void set_tooltip(const std::string &);
	const std::string &get_tooltip() const { return tooltip; }

	void set_visible(bool);
	bool is_visible() const { return visible; }
	void set_focus();

	void render() const;
protected:
	/*void render_graphic(const Part &) const;
	void render_text(const Part &, const std::string &) const;*/
	virtual void render_special(const Part &) const { }

public:
	// Events
	virtual void button_press(int, int, unsigned) { }
	virtual void button_release(int, int, unsigned) { }
	virtual void pointer_motion(int, int) { }
	virtual void pointer_enter();
	virtual void pointer_leave();
	virtual void key_press(unsigned, unsigned, wchar_t) { }
	virtual void key_release(unsigned, unsigned) { }
	virtual void focus_in();
	virtual void focus_out();

protected:
	/**
	Returns the name of the widget class.  Used for style lookup.
	*/
	virtual const char *get_class() const { return "widget"; }

	/**
	Gets a style object from the resource collection based on the class and
	style names of the widget.
	*/
	void update_style();

	/**
	Sets the widget's parent Panel.  The widget must be unparented when calling
	this function with a nonzero parameter.
	*/
	void set_parent(Container *);

	/**
	A helper function to set the parent of another widget.
	*/
	void set_parent(Widget &, Container *);

	// More events
	virtual void on_geometry_change() { }
	virtual void on_style_change() { }
	virtual void on_reparent() { }
};

} // namespace GLtk
} // namespace Msp

#endif
