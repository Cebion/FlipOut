/* $Id: loader.cpp 45 2008-08-06 16:35:21Z tdb $

This file is part of libmspdatafile
Copyright © 2006-2008  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include "loader.h"

using namespace std;

namespace Msp {
namespace DataFile {

void Loader::load(const Statement &st)
{
	for(list<Statement>::const_iterator i=st.sub.begin(); i!=st.sub.end(); ++i)
		load_statement(*i);
	finish();
}

void Loader::load(Parser &p)
{
	while(p)
	{
		Statement st=p.parse();
		if(st.valid)
			load_statement(st);
	}
	finish();
}

Loader::~Loader()
{
	for(ActionMap::iterator i=actions.begin(); i!=actions.end(); ++i)
		delete i->second;
}

void Loader::load_sub_with(Loader &ldr)
{
	if(!cur_st)
		throw InvalidState("load_sub called without current statement");

	ldr.load(*cur_st);
}

void Loader::add(const string &k, LoaderAction *a)
{
	ActionMap::iterator i=actions.find(k);
	if(i!=actions.end())
	{
		delete i->second;
		i->second=a;
	}
	else
		actions[k]=a;
}

void Loader::load_statement(const Statement &st)
{
	cur_st=&st;
	ActionMap::iterator j=actions.find(st.keyword);
	if(j==actions.end())
		throw_at(KeyError("Unknown keyword", st.keyword), st.get_location());
	if(j->second)
	{
		try
		{
			j->second->execute(*this, st);
		}
		catch(Exception &e)
		{
			if(!e.where()[0])
				e.at(st.get_location());
			throw;
		}
	}
	cur_st=0;
}

} // namespace DataFile
} // namespace Msp
