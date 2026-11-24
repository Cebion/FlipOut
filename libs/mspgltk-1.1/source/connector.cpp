/* $Id: connector.cpp 21 2007-12-26 09:20:37Z tdb $

This file is part of libmspgltk
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include "connector.h"
#include "logic.h"

using namespace std;

namespace Msp {
namespace GLtk {

Connector::~Connector()
{
	for(map<string, ConnAction *>::iterator i=actions.begin(); i!=actions.end(); ++i)
		delete i->second;
}

void Connector::connect(const Logic &logic)
{
	const list<Logic::WidgetBinding> &logic_binds=logic.get_bindings();

	for(list<Logic::WidgetBinding>::const_iterator i=logic_binds.begin(); i!=logic_binds.end(); ++i)
	{
		map<string, ConnAction *>::const_iterator j=actions.find(i->type);
		if(j!=actions.end())
			j->second->connect(*this, *i->wdg, i->data);
		else
			throw KeyError("Unknown binding type", i->type);
	}
}

void Connector::add(const string &type, ConnAction *act)
{
	map<string, ConnAction *>::iterator i=actions.find(type);
	if(i!=actions.end())
	{
		delete i->second;
		i->second=act;
	}
	else
		actions[type]=act;
}

} // namespace GLtk
} // namespace Msp
