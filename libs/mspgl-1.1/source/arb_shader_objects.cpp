#include "extension.h"
#include "arb_shader_objects.h"

namespace Msp {
namespace GL {

PFNGLDELETEOBJECTARBPROC glDeleteObjectARB=0;
PFNGLGETHANDLEARBPROC glGetHandleARB=0;
PFNGLDETACHOBJECTARBPROC glDetachObjectARB=0;
PFNGLCREATESHADEROBJECTARBPROC glCreateShaderObjectARB=0;
PFNGLSHADERSOURCEARBPROC glShaderSourceARB=0;
PFNGLCOMPILESHADERARBPROC glCompileShaderARB=0;
PFNGLCREATEPROGRAMOBJECTARBPROC glCreateProgramObjectARB=0;
PFNGLATTACHOBJECTARBPROC glAttachObjectARB=0;
PFNGLLINKPROGRAMARBPROC glLinkProgramARB=0;
PFNGLUSEPROGRAMOBJECTARBPROC glUseProgramObjectARB=0;
PFNGLVALIDATEPROGRAMARBPROC glValidateProgramARB=0;
PFNGLUNIFORM1FARBPROC glUniform1fARB=0;
PFNGLUNIFORM2FARBPROC glUniform2fARB=0;
PFNGLUNIFORM3FARBPROC glUniform3fARB=0;
PFNGLUNIFORM4FARBPROC glUniform4fARB=0;
PFNGLUNIFORM1IARBPROC glUniform1iARB=0;
PFNGLUNIFORM2IARBPROC glUniform2iARB=0;
PFNGLUNIFORM3IARBPROC glUniform3iARB=0;
PFNGLUNIFORM4IARBPROC glUniform4iARB=0;
PFNGLUNIFORM1FVARBPROC glUniform1fvARB=0;
PFNGLUNIFORM2FVARBPROC glUniform2fvARB=0;
PFNGLUNIFORM3FVARBPROC glUniform3fvARB=0;
PFNGLUNIFORM4FVARBPROC glUniform4fvARB=0;
PFNGLUNIFORM1IVARBPROC glUniform1ivARB=0;
PFNGLUNIFORM2IVARBPROC glUniform2ivARB=0;
PFNGLUNIFORM3IVARBPROC glUniform3ivARB=0;
PFNGLUNIFORM4IVARBPROC glUniform4ivARB=0;
PFNGLUNIFORMMATRIX2FVARBPROC glUniformMatrix2fvARB=0;
PFNGLUNIFORMMATRIX3FVARBPROC glUniformMatrix3fvARB=0;
PFNGLUNIFORMMATRIX4FVARBPROC glUniformMatrix4fvARB=0;
PFNGLGETOBJECTPARAMETERFVARBPROC glGetObjectParameterfvARB=0;
PFNGLGETOBJECTPARAMETERIVARBPROC glGetObjectParameterivARB=0;
PFNGLGETINFOLOGARBPROC glGetInfoLogARB=0;
PFNGLGETATTACHEDOBJECTSARBPROC glGetAttachedObjectsARB=0;
PFNGLGETUNIFORMLOCATIONARBPROC glGetUniformLocationARB=0;
PFNGLGETACTIVEUNIFORMARBPROC glGetActiveUniformARB=0;
PFNGLGETUNIFORMFVARBPROC glGetUniformfvARB=0;
PFNGLGETUNIFORMIVARBPROC glGetUniformivARB=0;
PFNGLGETSHADERSOURCEARBPROC glGetShaderSourceARB=0;

void init_arb_shader_objects()
{
	glDeleteObjectARB=reinterpret_cast<PFNGLDELETEOBJECTARBPROC>(get_proc_address("glDeleteObjectARB"));
	glGetHandleARB=reinterpret_cast<PFNGLGETHANDLEARBPROC>(get_proc_address("glGetHandleARB"));
	glDetachObjectARB=reinterpret_cast<PFNGLDETACHOBJECTARBPROC>(get_proc_address("glDetachObjectARB"));
	glCreateShaderObjectARB=reinterpret_cast<PFNGLCREATESHADEROBJECTARBPROC>(get_proc_address("glCreateShaderObjectARB"));
	glShaderSourceARB=reinterpret_cast<PFNGLSHADERSOURCEARBPROC>(get_proc_address("glShaderSourceARB"));
	glCompileShaderARB=reinterpret_cast<PFNGLCOMPILESHADERARBPROC>(get_proc_address("glCompileShaderARB"));
	glCreateProgramObjectARB=reinterpret_cast<PFNGLCREATEPROGRAMOBJECTARBPROC>(get_proc_address("glCreateProgramObjectARB"));
	glAttachObjectARB=reinterpret_cast<PFNGLATTACHOBJECTARBPROC>(get_proc_address("glAttachObjectARB"));
	glLinkProgramARB=reinterpret_cast<PFNGLLINKPROGRAMARBPROC>(get_proc_address("glLinkProgramARB"));
	glUseProgramObjectARB=reinterpret_cast<PFNGLUSEPROGRAMOBJECTARBPROC>(get_proc_address("glUseProgramObjectARB"));
	glValidateProgramARB=reinterpret_cast<PFNGLVALIDATEPROGRAMARBPROC>(get_proc_address("glValidateProgramARB"));
	glUniform1fARB=reinterpret_cast<PFNGLUNIFORM1FARBPROC>(get_proc_address("glUniform1fARB"));
	glUniform2fARB=reinterpret_cast<PFNGLUNIFORM2FARBPROC>(get_proc_address("glUniform2fARB"));
	glUniform3fARB=reinterpret_cast<PFNGLUNIFORM3FARBPROC>(get_proc_address("glUniform3fARB"));
	glUniform4fARB=reinterpret_cast<PFNGLUNIFORM4FARBPROC>(get_proc_address("glUniform4fARB"));
	glUniform1iARB=reinterpret_cast<PFNGLUNIFORM1IARBPROC>(get_proc_address("glUniform1iARB"));
	glUniform2iARB=reinterpret_cast<PFNGLUNIFORM2IARBPROC>(get_proc_address("glUniform2iARB"));
	glUniform3iARB=reinterpret_cast<PFNGLUNIFORM3IARBPROC>(get_proc_address("glUniform3iARB"));
	glUniform4iARB=reinterpret_cast<PFNGLUNIFORM4IARBPROC>(get_proc_address("glUniform4iARB"));
	glUniform1fvARB=reinterpret_cast<PFNGLUNIFORM1FVARBPROC>(get_proc_address("glUniform1fvARB"));
	glUniform2fvARB=reinterpret_cast<PFNGLUNIFORM2FVARBPROC>(get_proc_address("glUniform2fvARB"));
	glUniform3fvARB=reinterpret_cast<PFNGLUNIFORM3FVARBPROC>(get_proc_address("glUniform3fvARB"));
	glUniform4fvARB=reinterpret_cast<PFNGLUNIFORM4FVARBPROC>(get_proc_address("glUniform4fvARB"));
	glUniform1ivARB=reinterpret_cast<PFNGLUNIFORM1IVARBPROC>(get_proc_address("glUniform1ivARB"));
	glUniform2ivARB=reinterpret_cast<PFNGLUNIFORM2IVARBPROC>(get_proc_address("glUniform2ivARB"));
	glUniform3ivARB=reinterpret_cast<PFNGLUNIFORM3IVARBPROC>(get_proc_address("glUniform3ivARB"));
	glUniform4ivARB=reinterpret_cast<PFNGLUNIFORM4IVARBPROC>(get_proc_address("glUniform4ivARB"));
	glUniformMatrix2fvARB=reinterpret_cast<PFNGLUNIFORMMATRIX2FVARBPROC>(get_proc_address("glUniformMatrix2fvARB"));
	glUniformMatrix3fvARB=reinterpret_cast<PFNGLUNIFORMMATRIX3FVARBPROC>(get_proc_address("glUniformMatrix3fvARB"));
	glUniformMatrix4fvARB=reinterpret_cast<PFNGLUNIFORMMATRIX4FVARBPROC>(get_proc_address("glUniformMatrix4fvARB"));
	glGetObjectParameterfvARB=reinterpret_cast<PFNGLGETOBJECTPARAMETERFVARBPROC>(get_proc_address("glGetObjectParameterfvARB"));
	glGetObjectParameterivARB=reinterpret_cast<PFNGLGETOBJECTPARAMETERIVARBPROC>(get_proc_address("glGetObjectParameterivARB"));
	glGetInfoLogARB=reinterpret_cast<PFNGLGETINFOLOGARBPROC>(get_proc_address("glGetInfoLogARB"));
	glGetAttachedObjectsARB=reinterpret_cast<PFNGLGETATTACHEDOBJECTSARBPROC>(get_proc_address("glGetAttachedObjectsARB"));
	glGetUniformLocationARB=reinterpret_cast<PFNGLGETUNIFORMLOCATIONARBPROC>(get_proc_address("glGetUniformLocationARB"));
	glGetActiveUniformARB=reinterpret_cast<PFNGLGETACTIVEUNIFORMARBPROC>(get_proc_address("glGetActiveUniformARB"));
	glGetUniformfvARB=reinterpret_cast<PFNGLGETUNIFORMFVARBPROC>(get_proc_address("glGetUniformfvARB"));
	glGetUniformivARB=reinterpret_cast<PFNGLGETUNIFORMIVARBPROC>(get_proc_address("glGetUniformivARB"));
	glGetShaderSourceARB=reinterpret_cast<PFNGLGETSHADERSOURCEARBPROC>(get_proc_address("glGetShaderSourceARB"));
}

} // namespace GL
} // namespace Msp
