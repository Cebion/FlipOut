#ifndef MSP_GL_ARB_VERTEX_SHADER_
#define MSP_GL_ARB_VERTEX_SHADER_

#include "gl.h"
#include <GL/glext.h>

namespace Msp {
namespace GL {

extern PFNGLBINDATTRIBLOCATIONARBPROC glBindAttribLocationARB;
extern PFNGLGETACTIVEATTRIBARBPROC glGetActiveAttribARB;
extern PFNGLGETATTRIBLOCATIONARBPROC glGetAttribLocationARB;

void init_arb_vertex_shader();

} // namespace GL
} // namespace Msp

#endif
