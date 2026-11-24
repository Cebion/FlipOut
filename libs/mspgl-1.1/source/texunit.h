/* $Id: texunit.h 62 2008-09-24 16:10:43Z tdb $

This file is part of libmspgl
Copyright © 2007-2008  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_GL_TEXUNIT_H_
#define MSP_GL_TEXUNIT_H_

#include <vector>

namespace Msp {
namespace GL {

class TexEnv;
class Texture;

class TexUnit
{
public:
	TexUnit();
	bool set_texture(const Texture *);
	const Texture *get_texture() const { return texture; }
	bool set_texenv(const TexEnv *);
	const TexEnv *get_texenv() const { return texenv; }

	static TexUnit &activate(unsigned);
	static TexUnit &current();
private:
	const Texture *texture;
	const TexEnv *texenv;

	static std::vector<TexUnit> units;
	static TexUnit *cur_unit;
};

} // namespace GL
} // namespace Msp

#endif
