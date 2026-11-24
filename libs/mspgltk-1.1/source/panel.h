/* $Id: panel.h 55 2009-09-20 16:27:08Z tdb $

This file is part of libmspgltk
Copyright © 2007-2009  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_GLTK_PANEL_H_
#define MSP_GLTK_PANEL_H_

#include "container.h"

namespace Msp {
namespace GLtk {

/**
Panels are containers for other widgets.  Panel styles should have a special
part "children" to render the child widgets.  All properties of this part are
ignored.
*/
class Panel: public Container
{
public:
	class Loader: public Widget::Loader
	{
	private:
		Panel &pnl;
		std::map<std::string, Widget *> &wdg_map;
	
	public:
		Loader(Panel &, std::map<std::string, Widget *> &);
	private:
		template<typename T>
		void child(const std::string &);
		void panel(const std::string &);
	};

protected:
	struct Child: public Container::Child
	{
		Child(Panel &, Widget *);
		virtual ~Child();

		void visibility_changed(bool);
		void request_focus();
		void grab_pointer();
		void ungrab_pointer();
		void raise();
	};

	Widget *pointer_focus;
	bool pointer_grabbed;
	Widget *input_focus;

	Panel(const Panel &);
	Panel &operator=(const Panel &);
public:
	Panel(const Resources &);

	void raise(Widget &);

	virtual void button_press(int, int, unsigned);
	virtual void button_release(int, int, unsigned);
	virtual void pointer_motion(int, int);
	virtual void pointer_leave();
	virtual void key_press(unsigned, unsigned, wchar_t);
	virtual void key_release(unsigned, unsigned);
	virtual void focus_out();
protected:
	virtual const char *get_class() const { return "panel"; }
	virtual void render_special(const Part &) const;
	virtual Child *create_child(Widget *);

	void set_pointer_focus(Widget *);
	void set_input_focus(Widget *);
};

} // namespace GLtk
} // namespace Msp

#endif
