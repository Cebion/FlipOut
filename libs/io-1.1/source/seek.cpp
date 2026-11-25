/* $Id$

This file is part of libmspio
Copyright © 2007 Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/
#ifdef WIN32
#include <windows.h>
#endif
#include "except.h"
#include "seek.h"

namespace Msp {
namespace IO {

int sys_seek_type(SeekType st)
{
#ifdef WIN32
	if(st==S_BEG)
		return FILE_BEGIN;
	else if(st==S_CUR)
		return FILE_CURRENT;
	else if(st==S_END)
		return FILE_END;
#else
	if(st==S_BEG)
		return SEEK_SET;
	else if(st==S_CUR)
		return SEEK_CUR;
	else if(st==S_END)
		return SEEK_END;
#endif

	throw InvalidParameterValue("Invalid seek type");
}

} // namespace IO
} // namespace Msp
