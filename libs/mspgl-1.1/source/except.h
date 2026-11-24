/* $Id: except.h 31 2007-11-02 19:11:28Z tdb $

This file is part of libmspgl
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_GL_EXCEPT_H_
#define MSP_GL_EXCEPT_H_

#include <msp/core/except.h>

namespace Msp {
namespace GL {

class IncompatibleData: public Exception
{
public:
	IncompatibleData(const std::string &w_): Exception(w_) { }
	~IncompatibleData() throw() { }
};

class UnsupportedExtension: public Exception
{
public:
	UnsupportedExtension(const std::string &w_): Exception(w_+" is not supported") { }
	~UnsupportedExtension() throw() { }
};

class CompileError: public Exception
{
public:
	CompileError(const std::string &w_): Exception(w_) { }
	~CompileError() throw() { }
};

} // namespace GL
} // namespace Msp

#endif
