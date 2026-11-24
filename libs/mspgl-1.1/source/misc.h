/* $Id: misc.h 84 2009-09-21 13:50:58Z tdb $

This file is part of libmspgl
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_GL_MISC_H_
#define MSP_GL_MISC_H_

#include "gl.h"

namespace Msp {
namespace GL {

void enable(GLenum);
void disable(GLenum);
void set(GLenum, bool);

///Deprecated (can't properly pass an array through a reference)
void get(GLenum, int &);

void get(GLenum, int *);
int get_i(GLenum);

class Bind
{
private:
	struct Base
	{
		virtual ~Base() { }
	};

	template<typename T>
	struct Binder: Base
	{
		const T &obj;

		Binder(const T &o): obj(o) { obj.bind(); }
		~Binder() { obj.unbind(); }
	};

	Base *binder;

public:
	template<typename T>
	Bind(const T &o): binder(new Binder<T>(o)) { }
	~Bind() { delete binder; }
};

} // namespace GL
} // namespace Msp

#endif
