/* $Id: logic.cpp 21 2007-12-26 09:20:37Z tdb $

This file is part of libmspgltk
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include "logic.h"

using namespace std;

namespace Msp {
namespace GLtk {

Logic::Loader::Loader(Logic &l, const map<string, Widget *> &w):
	logic(l),
	widgets(w)
{
	add("bind", &Loader::bind);
}

void Logic::Loader::bind(const string &wdg, const string &data)
{
	map<string, Widget *>::const_iterator i=widgets.find(wdg);
	if(i==widgets.end())
		throw KeyError("Unknown widget", wdg);

	unsigned colon=data.find(':');
	WidgetBinding act;
	act.wdg=i->second;
	act.type=data.substr(0, colon);
	if(colon!=string::npos)
		act.data=data.substr(colon+1);
	logic.bindings.push_back(act);
}

} // namespace GLtk
} // namespace Msp
