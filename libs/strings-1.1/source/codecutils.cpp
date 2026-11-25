/* $Id$

This file is part of libmspstrings
Copyright © 2006-2007 Mikko Rasa
Distributed under the LGPL
*/

#include "codecutils.h"

namespace Msp {
namespace Codecs {

int transform_mapping_or_direct(const int *mapping, unsigned map_size, int ch, bool reverse)
{
	for(unsigned i=0; i<map_size*2; i+=2)
	{
		if(mapping[i+reverse]==ch)
			return mapping[i+1-reverse];
		else if(mapping[i+1-reverse]==ch)
			return -1;
	}

	return ch;
}

} // namespace Codecs
} // namespace Msp
