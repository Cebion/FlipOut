/* $Id: extension.h 61 2008-09-08 19:39:39Z tdb $

This file is part of libmspgl
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_GL_EXTENSION_H_
#define MSP_GL_EXTENSION_H_

#include <msp/core/except.h>

namespace Msp {
namespace GL {

struct Version
{
	unsigned short major;
	unsigned short minor;
};

typedef void ExtFunc();

/**
Indicates whether an extension is supported.  If this returns true, the
functions of that extension are safe to use.
*/
bool is_supported(const std::string &);

/**
Returns the OpenGL version number, as reported by the implementation.
Functions up to the returned version are safe to use.
*/
const Version &get_gl_version();

/**
Checks that an extension is supported and throws if it isn't.
*/
void require_extension(const std::string &);

/**
RAII version of require_extension.  Useful as a static local variable.
*/
struct RequireExtension
{
	RequireExtension(const std::string &e) { require_extension(e); }
};

/**
Checks that the OpenGL version is at least a.b and throws if it isn't.
*/
void require_version(unsigned a, unsigned b);

/**
RAII version of require_version.  Useful as a static local variable.
*/
struct RequireVersion
{
	RequireVersion(unsigned a, unsigned b) { require_version(a, b); }
};

/**
Returns the address of an extension function.
*/
ExtFunc *get_proc_address(const std::string &);

} // namespace GL
} // namespace Msp

#endif
