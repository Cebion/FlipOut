/* $Id: programdata.cpp 61 2008-09-08 19:39:39Z tdb $

This file is part of libmspgl
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include "extension.h"
#include "program.h"
#include "programdata.h"
#include "uniform.h"

using namespace std;

namespace Msp {
namespace GL {

ProgramData::ProgramData()
{
	static RequireExtension _ext("GL_ARB_shader_objects");
}

ProgramData::~ProgramData()
{
	for(map<int, Uniform *>::iterator i=data.begin(); i!=data.end(); ++i)
		delete i->second;
}

void ProgramData::uniform(int index, Uniform *uni)
{
	if(index<0)
	{
		delete uni;
		return;
	}

	map<int, Uniform *>::iterator i=data.find(index);
	if(i!=data.end())
	{
		delete i->second;
		i->second=uni;
	}
	else
		data[index]=uni;
}

void ProgramData::uniform(int index, int v)
{
	uniform(index, new Uniform1i(v));
}

void ProgramData::uniform(int index, float v)
{
	uniform(index, new Uniform1f(v));
}

void ProgramData::uniform(int index, float v0, float v1)
{
	uniform(index, new Uniform2f(v0, v1));
}

void ProgramData::uniform2(int index, const float *v)
{
	uniform(index, v[0], v[1]);
}

void ProgramData::uniform(int index, float v0, float v1, float v2)
{
	uniform(index, new Uniform3f(v0, v1, v2));
}

void ProgramData::uniform3(int index, const float *v)
{
	uniform(index, v[0], v[1], v[2]);
}

void ProgramData::uniform(int index, float v0, float v1, float v2, float v3)
{
	uniform(index, new Uniform4f(v0, v1, v2, v3));
}

void ProgramData::uniform4(int index, const float *v)
{
	uniform(index, v[0], v[1], v[2], v[3]);
}

void ProgramData::apply() const
{
	for(map<int, Uniform *>::const_iterator i=data.begin(); i!=data.end(); ++i)
		i->second->apply(i->first);
}


ProgramData::Loader::Loader(ProgramData &pd, Program &pr):
	pdata(pd),
	prog(pr)
{
	add("uniform1i", &Loader::uniform1i);
	add("uniform1f", &Loader::uniform1f);
	add("uniform2f", &Loader::uniform2f);
	add("uniform3f", &Loader::uniform3f);
	add("uniform4f", &Loader::uniform4f);
}

void ProgramData::Loader::uniform1i(const string &n, int v)
{
	pdata.uniform(prog.get_uniform_location(n), v);
}

void ProgramData::Loader::uniform1f(const string &n, float v)
{
	pdata.uniform(prog.get_uniform_location(n), v);
}

void ProgramData::Loader::uniform2f(const string &n, float v0, float v1)
{
	pdata.uniform(prog.get_uniform_location(n), v0, v1);
}

void ProgramData::Loader::uniform3f(const string &n, float v0, float v1, float v2)
{
	pdata.uniform(prog.get_uniform_location(n), v0, v1, v2);
}

void ProgramData::Loader::uniform4f(const string &n, float v0, float v1, float v2, float v3)
{
	pdata.uniform(prog.get_uniform_location(n), v0, v1, v2, v3);
}

} // namespace GL
} // namespace Msp
