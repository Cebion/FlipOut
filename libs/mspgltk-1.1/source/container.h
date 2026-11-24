/* $Id$

This file is part of libmspgltk
Copyright © 2009  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_GLTK_CONTAINER_H_
#define MSP_GLTK_CONTAINER_H_

#include <list>
#include <sigc++/trackable.h>
#include "widget.h"

namespace Msp {
namespace GLtk {

class Container: virtual public Widget
{
protected:
	struct Child: public sigc::trackable
	{
		Container &container;
		Widget *widget;

		Child(Container &, Widget *);
		virtual ~Child();

		void visibility_changed(bool);
	};

	std::list<Child *> children;
	Widget *click_focus;
	unsigned click_button;

	Container(const Resources &);
public:
	virtual ~Container();

	void add(Widget &);
	void remove(Widget &);
	std::list<Widget *> get_children() const;
	Widget *get_child_at(int, int);
	Widget *get_descendant_at(int, int);

	virtual void button_press(int, int, unsigned);
	virtual void button_release(int, int, unsigned);
	virtual void pointer_motion(int, int);
	virtual void pointer_leave();
protected:
	virtual Child *create_child(Widget *);
};

} // namespace GLtk
} // namespace Msp

#endif
