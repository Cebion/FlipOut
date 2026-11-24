/* $Id: statement.h 19 2007-08-21 14:11:23Z tdb $

This file is part of libmspdatafile
Copyright © 2006  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include <msp/strings/formatter.h>
#include "statement.h"

namespace Msp {
namespace DataFile {

std::string Statement::get_location() const
{
	std::string result=source;
	if(line)
		result+=format(":%d", line);
	return result;
}

} // namespace DataFile
} // namespace Msp
