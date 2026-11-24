/* $Id: shader.cpp 72 2009-01-10 10:07:36Z tdb $

This file is part of libmspgl
Copyright © 2007 Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include "arb_shader_objects.h"
#include "except.h"
#include "extension.h"
#include "shader.h"

using namespace std;

namespace Msp {
namespace GL {

Shader::Shader(ShaderType t)
{
	init(t);
}

Shader::Shader(ShaderType t, const string &src)
{
	init(t);

	source(src);
	compile();
}

void Shader::init(ShaderType t)
{
	compiled=false;

	if(t==FRAGMENT_SHADER)
		static RequireExtension _ext("GL_ARB_fragment_shader");
	else if(t==VERTEX_SHADER)
		static RequireExtension _ext("GL_ARB_vertex_shader");

	id=glCreateShaderObjectARB(t);
}

Shader::~Shader()
{
	glDeleteObjectARB(id);
}

void Shader::source(sizei count, const char **str, const int *len)
{
	glShaderSourceARB(id, count, str, len);
}

void Shader::source(const string &str)
{
	source(str.data(), str.size());
}

void Shader::source(const char *str, int len)
{
	source(1, &str, &len);
}

void Shader::compile()
{
	glCompileShaderARB(id);
	if(!(compiled=get_param(GL_COMPILE_STATUS)))
		throw CompileError(get_info_log());
}

int Shader::get_param(GLenum param) const
{
	int value;
	glGetObjectParameterivARB(id, param, &value);
	return value;
}

string Shader::get_info_log() const
{
	sizei len=get_param(GL_INFO_LOG_LENGTH);
	char log[len+1];
	glGetInfoLogARB(id, len+1, reinterpret_cast<GLsizei *>(&len), log);
	return string(log, len);
}

} // namespace GL
} // namespace Msp
