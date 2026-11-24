/* $Id: logic.h 21 2007-12-26 09:20:37Z tdb $

This file is part of libmspgltk
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_GLTK_LOGIC_H_
#define MSP_GLTK_LOGIC_H_

#include <list>
#include <map>
#include <string>
#include <sigc++/slot.h>
#include <msp/core/except.h>
#include <msp/datafile/loader.h>

namespace Msp {
namespace GLtk {

class Widget;

/**
Stores use interface logic.  This is represented as widget bindings.  Each
binding has type and data.

See also class Connector.
*/
class Logic
{
public:
	class Loader: public DataFile::Loader
	{
	private:
		Logic &logic;
		const std::map<std::string, Widget *> &widgets;

	public:
		Loader(Logic &, const std::map<std::string, Widget *> &);
	private:
		void bind(const std::string &, const std::string &);
	};

	struct WidgetBinding
	{
		Widget *wdg;
		std::string type;
		std::string data;
	};

private:
	std::list<WidgetBinding> bindings;

public:
	const std::list<WidgetBinding> &get_bindings() const { return bindings; }
};

} // namespace GLtk
} // namespace Msp

#endif
