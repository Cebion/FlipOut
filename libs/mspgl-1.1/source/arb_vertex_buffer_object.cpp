#include "extension.h"
#include "arb_vertex_buffer_object.h"

namespace Msp {
namespace GL {

PFNGLBINDBUFFERARBPROC glBindBufferARB=0;
PFNGLDELETEBUFFERSARBPROC glDeleteBuffersARB=0;
PFNGLGENBUFFERSARBPROC glGenBuffersARB=0;
PFNGLISBUFFERARBPROC glIsBufferARB=0;
PFNGLBUFFERDATAARBPROC glBufferDataARB=0;
PFNGLBUFFERSUBDATAARBPROC glBufferSubDataARB=0;
PFNGLGETBUFFERSUBDATAARBPROC glGetBufferSubDataARB=0;
PFNGLMAPBUFFERARBPROC glMapBufferARB=0;
PFNGLUNMAPBUFFERARBPROC glUnmapBufferARB=0;
PFNGLGETBUFFERPARAMETERIVARBPROC glGetBufferParameterivARB=0;
PFNGLGETBUFFERPOINTERVARBPROC glGetBufferPointervARB=0;

void init_arb_vertex_buffer_object()
{
	glBindBufferARB=reinterpret_cast<PFNGLBINDBUFFERARBPROC>(get_proc_address("glBindBufferARB"));
	glDeleteBuffersARB=reinterpret_cast<PFNGLDELETEBUFFERSARBPROC>(get_proc_address("glDeleteBuffersARB"));
	glGenBuffersARB=reinterpret_cast<PFNGLGENBUFFERSARBPROC>(get_proc_address("glGenBuffersARB"));
	glIsBufferARB=reinterpret_cast<PFNGLISBUFFERARBPROC>(get_proc_address("glIsBufferARB"));
	glBufferDataARB=reinterpret_cast<PFNGLBUFFERDATAARBPROC>(get_proc_address("glBufferDataARB"));
	glBufferSubDataARB=reinterpret_cast<PFNGLBUFFERSUBDATAARBPROC>(get_proc_address("glBufferSubDataARB"));
	glGetBufferSubDataARB=reinterpret_cast<PFNGLGETBUFFERSUBDATAARBPROC>(get_proc_address("glGetBufferSubDataARB"));
	glMapBufferARB=reinterpret_cast<PFNGLMAPBUFFERARBPROC>(get_proc_address("glMapBufferARB"));
	glUnmapBufferARB=reinterpret_cast<PFNGLUNMAPBUFFERARBPROC>(get_proc_address("glUnmapBufferARB"));
	glGetBufferParameterivARB=reinterpret_cast<PFNGLGETBUFFERPARAMETERIVARBPROC>(get_proc_address("glGetBufferParameterivARB"));
	glGetBufferPointervARB=reinterpret_cast<PFNGLGETBUFFERPOINTERVARBPROC>(get_proc_address("glGetBufferPointervARB"));
}

} // namespace GL
} // namespace Msp
