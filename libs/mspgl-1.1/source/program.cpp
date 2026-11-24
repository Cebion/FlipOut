/* $Id: program.cpp 77 2009-05-24 10:56:35Z tdb $

This file is part of libmspgl
Copyright © 2007 Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include <algorithm>
#include "arb_shader_objects.h"
#include "arb_vertex_shader.h"
#include "except.h"
#include "extension.h"
#include "program.h"
#include "shader.h"

using namespace std;

namespace Msp {
namespace GL {

Program::Program():
	del_shaders(false)
{
	init();
}

Program::Program(const string &vert, const string &frag):
	del_shaders(true)
{
	init();

	attach_shader(*new Shader(VERTEX_SHADER, vert));
	attach_shader(*new Shader(FRAGMENT_SHADER, frag));
	link();
}

void Program::init()
{
	static RequireExtension _ext("GL_ARB_shader_objects");

	linked=false;
	id=glCreateProgramObjectARB();
}

Program::~Program()
{
	if(del_shaders)
	{
		for(list<Shader *>::iterator i=shaders.begin(); i!=shaders.end(); ++i)
			delete *i;
	}
	glDeleteObjectARB(id);
}

void Program::attach_shader(Shader &shader)
{
	if(find(shaders.begin(), shaders.end(), &shader)==shaders.end())
	{
		glAttachObjectARB(id, shader.get_id());
		shaders.push_back(&shader);
	}
}

void Program::detach_shader(Shader &shader)
{
	list<Shader *>::iterator i=remove(shaders.begin(), shaders.end(), &shader);
	if(i!=shaders.end())
	{
		shaders.erase(i, shaders.end());
		glDetachObjectARB(id, shader.get_id());
	}
}

void Program::set_del_shaders(bool ds)
{
	del_shaders=ds;
}

void Program::bind_attribute(uint index, const string &name)
{
	static RequireExtension _ext("GL_ARB_vertex_shader");
	glBindAttribLocationARB(id, index, name.c_str());
}

void Program::link()
{
	for(list<Shader *>::iterator i=shaders.begin(); i!=shaders.end(); ++i)
		if(!(*i)->get_compiled())
			(*i)->compile();

	glLinkProgramARB(id);
	if(!(linked=get_param(GL_LINK_STATUS)))
		throw CompileError(get_info_log());
}

int Program::get_param(GLenum param) const
{
	int value;
	glGetObjectParameterivARB(id, param, &value);
	return value;
}

string Program::get_info_log() const
{
	sizei len=get_param(GL_INFO_LOG_LENGTH);
	char log[len+1];
	glGetInfoLogARB(id, len+1, reinterpret_cast<GLsizei *>(&len), log);
	return string(log, len);
}

void Program::bind() const
{
	if(!linked)
		throw InvalidState("Program is not linked");

	glUseProgramObjectARB(id);
	cur_prog=this;
}

int Program::get_uniform_location(const string &n) const
{
	return glGetUniformLocationARB(id, n.c_str());
}

void Program::unbind()
{
	if(cur_prog)
	{
		glUseProgramObjectARB(0);
		cur_prog=0;
	}
}

void Program::maybe_bind()
{
	if(cur_prog!=this)
		bind();
}

const Program *Program::cur_prog=0;


Program::Loader::Loader(Program &p):
	prog(p)
{
	prog.set_del_shaders(true);

	add("vertex_shader",   &Loader::vertex_shader);
	add("fragment_shader", &Loader::fragment_shader);
	add("attribute",       &Loader::attribute);
}

void Program::Loader::vertex_shader(const string &src)
{
	prog.attach_shader(*new Shader(VERTEX_SHADER, src));
}

void Program::Loader::fragment_shader(const string &src)
{
	prog.attach_shader(*new Shader(FRAGMENT_SHADER, src));
}

void Program::Loader::attribute(uint i, const string &n)
{
	prog.bind_attribute(i, n);
}

void Program::Loader::finish()
{
	prog.link();
}

} // namespace GL
} // namespace Msp
