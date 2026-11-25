/* $Id$

This file is part of libmspio
Copyright © 2007 Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/
#include "base.h"
#include "utils.h"

namespace Msp {
namespace IO {

unsigned read_all(Base &obj, char *buf, unsigned size)
{
	unsigned pos=0;
	while(pos<size)
		pos+=obj.read(buf+pos, size-pos);

	return pos;
}

} // namespace IO
} // namespace Msp
