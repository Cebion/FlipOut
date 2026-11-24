#include "extension.h"
#include "arb_vertex_shader.h"

namespace Msp {
namespace GL {

PFNGLBINDATTRIBLOCATIONARBPROC glBindAttribLocationARB=0;
PFNGLGETACTIVEATTRIBARBPROC glGetActiveAttribARB=0;
PFNGLGETATTRIBLOCATIONARBPROC glGetAttribLocationARB=0;

void init_arb_vertex_shader()
{
	glBindAttribLocationARB=reinterpret_cast<PFNGLBINDATTRIBLOCATIONARBPROC>(get_proc_address("glBindAttribLocationARB"));
	glGetActiveAttribARB=reinterpret_cast<PFNGLGETACTIVEATTRIBARBPROC>(get_proc_address("glGetActiveAttribARB"));
	glGetAttribLocationARB=reinterpret_cast<PFNGLGETATTRIBLOCATIONARBPROC>(get_proc_address("glGetAttribLocationARB"));
}

} // namespace GL
} // namespace Msp
