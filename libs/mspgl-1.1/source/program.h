/* $Id: program.h 77 2009-05-24 10:56:35Z tdb $

This file is part of libmspgl
Copyright © 2007 Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_GL_PROGRAM_H_
#define MSP_GL_PROGRAM_H_

#include <list>
#include <string>
#include <msp/datafile/loader.h>
#include "gl.h"
#include "types.h"

namespace Msp {
namespace GL {

class Shader;

class Program
{
private:
	uint id;
	std::list<Shader *> shaders;
	bool del_shaders;
	bool linked;

	static const Program *cur_prog;

public:
	class Loader: public DataFile::Loader
	{
	private:
		Program &prog;

	public:
		Loader(Program &);

	private:
		void vertex_shader(const std::string &);
		void fragment_shader(const std::string &);
		void attribute(uint, const std::string &);
		virtual void finish();
	};

	Program();
	Program(const std::string &, const std::string &);
private:
	void init();
public:
	virtual ~Program();

	void attach_shader(Shader &shader);
	void detach_shader(Shader &shader);
	const std::list<Shader *> &get_shaders() const { return shaders; }
	void set_del_shaders(bool);
	void bind_attribute(uint, const std::string &);
	void link();
	int get_param(GLenum param) const;
	bool get_linked() const { return linked; }
	std::string get_info_log() const;
	void bind() const;
	int get_uniform_location(const std::string &) const;

	static void unbind();

private:
	void maybe_bind();
};

} // namespace GL
} // namespace Msp

#endif
