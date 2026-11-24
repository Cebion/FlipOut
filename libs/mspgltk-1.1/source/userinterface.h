/* $Id: userinterface.h 35 2008-07-20 19:11:43Z tdb $

This file is part of libmspgltk
Copyright © 2008  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_GLTK_USERINTERFACE_H_
#define MSP_GLTK_USERINTERFACE_H_

#include "logic.h"
#include "root.h"

namespace Msp {
namespace GLtk {

/**
Encapsulates a Root widget and Logic associated with it.  Allows looking up
widgets by name.
*/
class UserInterface
{
public:
	class Loader: public DataFile::Loader
	{
	private:
		UserInterface &ui;

	public:
		Loader(UserInterface &);
	private:
		void logic();
		void root();
	};

private:
	std::map<std::string, Widget *> widgets;
	Root root;
	Logic logic;

public:
	UserInterface(Resources &, Graphics::Window &);
	Root &get_root() { return root; }
	const Logic &get_logic() const { return logic; }

	template<typename W>
	W &get_widget(const std::string &n) const
	{
		std::map<std::string, Widget *>::const_iterator i=widgets.find(n);
		if(i==widgets.end())
			throw KeyError("Unknown widget", n);

		W *w=dynamic_cast<W *>(i->second);
		if(!w)
			throw Exception("Widget type mismatch");

		return *w;
	}

	template<typename W>
	void get_widget(const std::string &n, W *&w) const
	{
		w=&get_widget<W>(n);
	}
};

} // namespace GLtk
} // namespace Msp

#endif
