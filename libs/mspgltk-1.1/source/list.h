/* $Id: list.h 54 2009-09-16 19:16:18Z tdb $

This file is part of libmspgltk
Copyright © 2007-2009  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_GLTK_LIST_H_
#define MSP_GLTK_LIST_H_

#include <sigc++/signal.h>
#include "container.h"
#include "vslider.h"

namespace Msp {
namespace GLtk {

/**
Shows a list of items, allowing the user to select one.  A slider is included
to allow scrolling through a long list.
*/
class List: virtual public Widget, private Container
{
public:
	class Loader: public Widget::Loader
	{
	public:
		Loader(List &);
	private:
		void item(const std::string &);
	};

private:
	std::vector<std::string> items;
	int sel_index;
	unsigned first;
	unsigned n_visible;
	unsigned row_height;

	const Part *items_part;

	VSlider slider;

public:
	sigc::signal<void, unsigned, const std::string &> signal_item_selected;

	List(const Resources &);

	void append(const std::string &);
	void insert(unsigned, const std::string &);
	void remove(unsigned);
	void clear();
	unsigned get_n_items() const { return items.size(); }

	void set_selected_index(int);
	const std::string &get_selected() const;
	int get_selected_index() const { return sel_index; }

	virtual void button_press(int, int, unsigned);

private:
	virtual const char *get_class() const { return "list"; }
	virtual void render_special(const Part &) const;

	virtual void on_geometry_change();
	virtual void on_style_change();
	void reposition_slider();
	void recalculate_parameters();
	void slider_value_changed(double);
};

} // namespace GLtk
} // namespace Msp

#endif
