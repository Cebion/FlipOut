/* $Id: state.cpp 27 2008-03-03 17:48:55Z tdb $

This file is part of libmspgltk
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include "state.h"

using namespace std;

namespace Msp {
namespace GLtk {

istream &operator>>(istream &is, State &state)
{
	string str;
	is>>str;

	unsigned start=0;
	state=NORMAL;

	while(1)
	{
		unsigned underscore=str.find('_', start);
		if(!str.compare(start, underscore-start, "NORMAL"))
			state|=NORMAL;
		else if(!str.compare(start, underscore-start, "HOVER"))
			state|=HOVER;
		else if(!str.compare(start, underscore-start, "ACTIVE"))
			state|=ACTIVE;
		else if(!str.compare(start, underscore-start, "FOCUS"))
			state|=FOCUS;
		else if(!str.compare(start, underscore-start, "DISABLED"))
			state|=DISABLED;
		else
		{
			is.setstate(ios_base::failbit);
			break;
		}

		if(underscore==std::string::npos)
			break;
		start=underscore+1;
	}

	return is;
}

} // namespace GLtk
} // namespace Msp
