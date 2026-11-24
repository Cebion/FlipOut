/* $Id: shader.h 61 2008-09-08 19:39:39Z tdb $

This file is part of libmspgl
Copyright © 2007 Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_GL_SHADER_H_
#define MSP_GL_SHADER_H_

#include <string>
#include "gl.h"
#include "types.h"

namespace Msp {
namespace GL {

enum ShaderType
{
	FRAGMENT_SHADER = GL_FRAGMENT_SHADER,
	VERTEX_SHADER   = GL_VERTEX_SHADER
};

class Shader
{
public:
	Shader(ShaderType t);
	Shader(ShaderType t, const std::string &);
private:
	void init(ShaderType);
public:
	~Shader();

	void source(sizei count, const char **str, const int *len);
	void source(const std::string &str);
	void source(const char *str, int len);
	void compile();
	uint get_id() const { return id; }
	bool get_compiled() const { return compiled; }
	int get_param(GLenum param) const;
	std::string get_info_log() const;
private:
	uint id;
	bool compiled;
};

} // namespace GL
} // namespace Msp

#endif
