/* $Id: label.h 55 2009-09-20 16:27:08Z tdb $

This file is part of libmspgltk
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_GLTK_LABEL_H_
#define MSP_GLTK_LABEL_H_

#include "text.h"
#include "widget.h"

namespace Msp {
namespace GLtk {

/**
Labels display static text.  There is one special part: "text".
*/
class Label: public Widget
{
public:
	class Loader: public Widget::Loader
	{
	public:
		Loader(Label &);
	private:
		void text(const std::string &);
	};

private:
	Text text;

public:
	Label(const Resources &, const std::string & =std::string());

	virtual void autosize();
	void set_text(const std::string &);

private:
	virtual const char *get_class() const { return "label"; }
	virtual void render_special(const Part &) const;
};

} // namespace GLtk
} // namespace Msp

#endif
