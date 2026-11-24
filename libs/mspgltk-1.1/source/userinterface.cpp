/* $Id: userinterface.cpp 35 2008-07-20 19:11:43Z tdb $

This file is part of libmspgltk
Copyright © 2008  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include "userinterface.h"

namespace Msp {
namespace GLtk {

UserInterface::UserInterface(Resources &r, Graphics::Window &w):
	root(r, w)
{ }


UserInterface::Loader::Loader(UserInterface &u):
	ui(u)
{
	add("logic", &Loader::logic);
	add("root",  &Loader::root);
}

void UserInterface::Loader::logic()
{
	load_sub(ui.logic, ui.widgets);
}

void UserInterface::Loader::root()
{
	load_sub(ui.root, ui.widgets);
}

} // namespace GLtk
} // namespace Msp
