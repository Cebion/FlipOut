/* $Id: condition.cpp 97 2009-09-20 16:19:05Z tdb $

This file is part of builder
Copyright © 2007-2009  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include <msp/strings/utils.h>
#include "builder.h"
#include "condition.h"
#include "sourcepackage.h"

using namespace std;
using namespace Msp;

Condition::Condition(SourcePackage &p, const string &expr):
	pkg(p)
{
	vector<string> parts=split(expr);

	for(vector<string>::iterator i=parts.begin(); i!=parts.end(); ++i)
	{
		if(*i=="and")
			continue;

		string::size_type token=i->find_first_of("=!");
		if(token==string::npos)
			expression.insert(StringMap::value_type(*i, "!0"));
		else if(token==0 && (*i)[0]=='!')
			expression.insert(StringMap::value_type(*i, "=0"));
		else
			expression.insert(StringMap::value_type(i->substr(0, token), i->substr(token)));
	}
}

bool Condition::eval()
{
	const Config &conf=pkg.get_config();

	bool result=true;
	for(StringMap::iterator i=expression.begin(); i!=expression.end(); ++i)
	{
		bool neg=(i->second[0]=='!');
		unsigned start=1;
		if(i->second[1]=='=')
			++start;

		string value;
		if(conf.is_option(i->first))
			value=conf.get_option(i->first).value;
		else if(i->first=="arch")
			value=pkg.get_builder().get_current_arch().get_name();

		if((value==i->second.substr(start))==neg)
			result=false;
	}

	return result;
}


Condition::Loader::Loader(Condition &c):
	cond(c)
{
	add("require",    &Loader::require);
	add("build_info", &Loader::build_info);
}

void Condition::Loader::require(const string &pkg)
{
	cond.requires.push_back(pkg);
}

void Condition::Loader::build_info()
{
	load_sub(cond.build_info);
}
