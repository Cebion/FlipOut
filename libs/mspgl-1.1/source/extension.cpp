/* $Id: extension.cpp 72 2009-01-10 10:07:36Z tdb $

This file is part of libmspgl
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include <set>
#ifndef WIN32
#include <GL/glx.h>
#endif
#include <msp/strings/formatter.h>
#include <msp/strings/utils.h>
#include "arb_shader_objects.h"
#include "arb_vertex_buffer_object.h"
#include "arb_vertex_program.h"
#include "arb_vertex_shader.h"
#include "ext_framebuffer_object.h"
#include "except.h"
#include "extension.h"
#include "gl.h"
#include "version_1_2.h"
#include "version_1_3.h"

using namespace std;

namespace Msp {
namespace GL {

bool is_supported(const string &ext)
{
	static set<string> extensions;
	static bool init_done=false;

	if(!init_done)
	{
		if(const char *gl_ext=reinterpret_cast<const char *>(glGetString(GL_EXTENSIONS)))
		{
			vector<string> exts=split(gl_ext);
			extensions.insert(exts.begin(), exts.end());
		}

		/* XXX Conceptually a bit weird place for this, but I couldn't really come up
		   with anything better that would still be transparent. */
		if(extensions.count("GL_ARB_shader_objects"))
			init_arb_shader_objects();
		if(extensions.count("GL_ARB_vertex_shader"))
			init_arb_vertex_shader();
		if(extensions.count("GL_ARB_vertex_program"))
			init_arb_vertex_program();
		if(extensions.count("GL_EXT_framebuffer_object"))
			init_ext_framebuffer_object();
		if(extensions.count("GL_ARB_vertex_buffer_object"))
			init_arb_vertex_buffer_object();

		init_done=true;
	}

	return extensions.count(ext);
}

const Version &get_gl_version()
{
	static Version version;
	static bool init_done=false;

	if(!init_done)
	{
		string gl_ver=reinterpret_cast<const char *>(glGetString(GL_VERSION));
		vector<string> parts=split(gl_ver.substr(0, gl_ver.find(' ')), '.');
		version.major=lexical_cast<unsigned>(parts[0]);
		version.minor=lexical_cast<unsigned>(parts[1]);

		unsigned combined=version.major*0x100+version.minor;
		if(combined>=0x102)
			init_version_1_2();
		if(combined>=0x103)
			init_version_1_3();
	}

	return version;
}

void require_extension(const string &ext)
{
	if(!is_supported(ext))
		throw UnsupportedExtension(ext);
}

void require_version(unsigned a, unsigned b)
{
	const Version &ver=get_gl_version();
	if(ver.major<a || (ver.major==a && ver.minor<b))
		throw UnsupportedExtension(format("OpenGL %d.%d", a, b));
}

ExtFunc *get_proc_address(const string &name)
{
#ifndef WIN32
	return glXGetProcAddressARB(reinterpret_cast<const GLubyte *>(name.c_str()));
#else
	return reinterpret_cast<ExtFunc *>(wglGetProcAddress(name.c_str()));
#endif
}

} // namespace GL
} // namespace Msp
