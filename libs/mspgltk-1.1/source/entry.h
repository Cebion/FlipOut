/* $Id: entry.h 55 2009-09-20 16:27:08Z tdb $

This file is part of libmspgltk
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_GLTK_ENTRY_H_
#define MSP_GLTK_ENTRY_H_

#include "text.h"
#include "widget.h"

namespace Msp {
namespace GLtk {

/**
Text entry field.

Special parts:

  text    The current text of the widget.  Graphics are ignored.
  cursor  Indicates the current input position.  Fill_x is ignored.
*/
class Entry: public Widget
{
public:
	class Loader: public Widget::Loader
	{
	public:
		Loader(Entry &);
	};

private:
	Text text;
	unsigned edit_pos;

public:
	sigc::signal<void> signal_enter;

	Entry(const Resources &, const std::string & =std::string());

	void set_text(const std::string &);
	const std::string &get_text() const { return text.get(); }

	virtual void key_press(unsigned, unsigned, wchar_t);
private:
	virtual const char *get_class() const { return "entry"; }
	virtual void render_special(const Part &) const;
};

} // namespace GLtk
} // namespace Msp

#endif

