/* $Id: dropdown.h 54 2009-09-16 19:16:18Z tdb $

This file is part of libmspgltk
Copyright © 2007-2009  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_GLTK_DROPDOWN_H_
#define MSP_GLTK_DROPDOWN_H_

#include <sigc++/signal.h>
#include "list.h"
#include "widget.h"

namespace Msp {
namespace GLtk {

class List;

class Dropdown: virtual public Widget, private Container
{
public:
	class Loader: public Widget::Loader
	{
	public:
		Loader(Dropdown &);
	private:
		void item(const std::string &);
	};

private:
	List list;
	bool dropped;

public:
	sigc::signal<void, int, const std::string &> signal_item_selected;

	Dropdown(const Resources &);

	void append(const std::string &);
	void insert(unsigned, const std::string &);
	void remove(unsigned);
	void clear();
	unsigned get_n_items() const;

	void set_selected_index(int);
	const std::string &get_selected() const;
	int get_selected_index() const;

	virtual void button_press(int, int, unsigned);

private:
	virtual const char *get_class() const { return "dropdown"; }
	virtual void render_special(const Part &) const;

	virtual void on_geometry_change();
	void resize_list();

	void list_item_selected(unsigned, const std::string &);
};

} // namespace GLtk
} // namespace Msp

#endif
