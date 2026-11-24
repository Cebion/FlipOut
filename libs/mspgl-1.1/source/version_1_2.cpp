#include "extension.h"
#include "version_1_2.h"

namespace Msp {
namespace GL {

PFNGLBLENDCOLORPROC glBlendColor=0;
PFNGLBLENDEQUATIONPROC glBlendEquation=0;
PFNGLDRAWRANGEELEMENTSPROC glDrawRangeElements=0;
PFNGLCOLORTABLEPROC glColorTable=0;
PFNGLCOLORTABLEPARAMETERFVPROC glColorTableParameterfv=0;
PFNGLCOLORTABLEPARAMETERIVPROC glColorTableParameteriv=0;
PFNGLCOPYCOLORTABLEPROC glCopyColorTable=0;
PFNGLGETCOLORTABLEPROC glGetColorTable=0;
PFNGLGETCOLORTABLEPARAMETERFVPROC glGetColorTableParameterfv=0;
PFNGLGETCOLORTABLEPARAMETERIVPROC glGetColorTableParameteriv=0;
PFNGLCOLORSUBTABLEPROC glColorSubTable=0;
PFNGLCOPYCOLORSUBTABLEPROC glCopyColorSubTable=0;
PFNGLCONVOLUTIONFILTER1DPROC glConvolutionFilter1D=0;
PFNGLCONVOLUTIONFILTER2DPROC glConvolutionFilter2D=0;
PFNGLCONVOLUTIONPARAMETERFPROC glConvolutionParameterf=0;
PFNGLCONVOLUTIONPARAMETERFVPROC glConvolutionParameterfv=0;
PFNGLCONVOLUTIONPARAMETERIPROC glConvolutionParameteri=0;
PFNGLCONVOLUTIONPARAMETERIVPROC glConvolutionParameteriv=0;
PFNGLCOPYCONVOLUTIONFILTER1DPROC glCopyConvolutionFilter1D=0;
PFNGLCOPYCONVOLUTIONFILTER2DPROC glCopyConvolutionFilter2D=0;
PFNGLGETCONVOLUTIONFILTERPROC glGetConvolutionFilter=0;
PFNGLGETCONVOLUTIONPARAMETERFVPROC glGetConvolutionParameterfv=0;
PFNGLGETCONVOLUTIONPARAMETERIVPROC glGetConvolutionParameteriv=0;
PFNGLGETSEPARABLEFILTERPROC glGetSeparableFilter=0;
PFNGLSEPARABLEFILTER2DPROC glSeparableFilter2D=0;
PFNGLGETHISTOGRAMPROC glGetHistogram=0;
PFNGLGETHISTOGRAMPARAMETERFVPROC glGetHistogramParameterfv=0;
PFNGLGETHISTOGRAMPARAMETERIVPROC glGetHistogramParameteriv=0;
PFNGLGETMINMAXPROC glGetMinmax=0;
PFNGLGETMINMAXPARAMETERFVPROC glGetMinmaxParameterfv=0;
PFNGLGETMINMAXPARAMETERIVPROC glGetMinmaxParameteriv=0;
PFNGLHISTOGRAMPROC glHistogram=0;
PFNGLMINMAXPROC glMinmax=0;
PFNGLRESETHISTOGRAMPROC glResetHistogram=0;
PFNGLRESETMINMAXPROC glResetMinmax=0;
PFNGLTEXIMAGE3DPROC glTexImage3D=0;
PFNGLTEXSUBIMAGE3DPROC glTexSubImage3D=0;
PFNGLCOPYTEXSUBIMAGE3DPROC glCopyTexSubImage3D=0;

void init_version_1_2()
{
	glBlendColor=reinterpret_cast<PFNGLBLENDCOLORPROC>(get_proc_address("glBlendColor"));
	glBlendEquation=reinterpret_cast<PFNGLBLENDEQUATIONPROC>(get_proc_address("glBlendEquation"));
	glDrawRangeElements=reinterpret_cast<PFNGLDRAWRANGEELEMENTSPROC>(get_proc_address("glDrawRangeElements"));
	glColorTable=reinterpret_cast<PFNGLCOLORTABLEPROC>(get_proc_address("glColorTable"));
	glColorTableParameterfv=reinterpret_cast<PFNGLCOLORTABLEPARAMETERFVPROC>(get_proc_address("glColorTableParameterfv"));
	glColorTableParameteriv=reinterpret_cast<PFNGLCOLORTABLEPARAMETERIVPROC>(get_proc_address("glColorTableParameteriv"));
	glCopyColorTable=reinterpret_cast<PFNGLCOPYCOLORTABLEPROC>(get_proc_address("glCopyColorTable"));
	glGetColorTable=reinterpret_cast<PFNGLGETCOLORTABLEPROC>(get_proc_address("glGetColorTable"));
	glGetColorTableParameterfv=reinterpret_cast<PFNGLGETCOLORTABLEPARAMETERFVPROC>(get_proc_address("glGetColorTableParameterfv"));
	glGetColorTableParameteriv=reinterpret_cast<PFNGLGETCOLORTABLEPARAMETERIVPROC>(get_proc_address("glGetColorTableParameteriv"));
	glColorSubTable=reinterpret_cast<PFNGLCOLORSUBTABLEPROC>(get_proc_address("glColorSubTable"));
	glCopyColorSubTable=reinterpret_cast<PFNGLCOPYCOLORSUBTABLEPROC>(get_proc_address("glCopyColorSubTable"));
	glConvolutionFilter1D=reinterpret_cast<PFNGLCONVOLUTIONFILTER1DPROC>(get_proc_address("glConvolutionFilter1D"));
	glConvolutionFilter2D=reinterpret_cast<PFNGLCONVOLUTIONFILTER2DPROC>(get_proc_address("glConvolutionFilter2D"));
	glConvolutionParameterf=reinterpret_cast<PFNGLCONVOLUTIONPARAMETERFPROC>(get_proc_address("glConvolutionParameterf"));
	glConvolutionParameterfv=reinterpret_cast<PFNGLCONVOLUTIONPARAMETERFVPROC>(get_proc_address("glConvolutionParameterfv"));
	glConvolutionParameteri=reinterpret_cast<PFNGLCONVOLUTIONPARAMETERIPROC>(get_proc_address("glConvolutionParameteri"));
	glConvolutionParameteriv=reinterpret_cast<PFNGLCONVOLUTIONPARAMETERIVPROC>(get_proc_address("glConvolutionParameteriv"));
	glCopyConvolutionFilter1D=reinterpret_cast<PFNGLCOPYCONVOLUTIONFILTER1DPROC>(get_proc_address("glCopyConvolutionFilter1D"));
	glCopyConvolutionFilter2D=reinterpret_cast<PFNGLCOPYCONVOLUTIONFILTER2DPROC>(get_proc_address("glCopyConvolutionFilter2D"));
	glGetConvolutionFilter=reinterpret_cast<PFNGLGETCONVOLUTIONFILTERPROC>(get_proc_address("glGetConvolutionFilter"));
	glGetConvolutionParameterfv=reinterpret_cast<PFNGLGETCONVOLUTIONPARAMETERFVPROC>(get_proc_address("glGetConvolutionParameterfv"));
	glGetConvolutionParameteriv=reinterpret_cast<PFNGLGETCONVOLUTIONPARAMETERIVPROC>(get_proc_address("glGetConvolutionParameteriv"));
	glGetSeparableFilter=reinterpret_cast<PFNGLGETSEPARABLEFILTERPROC>(get_proc_address("glGetSeparableFilter"));
	glSeparableFilter2D=reinterpret_cast<PFNGLSEPARABLEFILTER2DPROC>(get_proc_address("glSeparableFilter2D"));
	glGetHistogram=reinterpret_cast<PFNGLGETHISTOGRAMPROC>(get_proc_address("glGetHistogram"));
	glGetHistogramParameterfv=reinterpret_cast<PFNGLGETHISTOGRAMPARAMETERFVPROC>(get_proc_address("glGetHistogramParameterfv"));
	glGetHistogramParameteriv=reinterpret_cast<PFNGLGETHISTOGRAMPARAMETERIVPROC>(get_proc_address("glGetHistogramParameteriv"));
	glGetMinmax=reinterpret_cast<PFNGLGETMINMAXPROC>(get_proc_address("glGetMinmax"));
	glGetMinmaxParameterfv=reinterpret_cast<PFNGLGETMINMAXPARAMETERFVPROC>(get_proc_address("glGetMinmaxParameterfv"));
	glGetMinmaxParameteriv=reinterpret_cast<PFNGLGETMINMAXPARAMETERIVPROC>(get_proc_address("glGetMinmaxParameteriv"));
	glHistogram=reinterpret_cast<PFNGLHISTOGRAMPROC>(get_proc_address("glHistogram"));
	glMinmax=reinterpret_cast<PFNGLMINMAXPROC>(get_proc_address("glMinmax"));
	glResetHistogram=reinterpret_cast<PFNGLRESETHISTOGRAMPROC>(get_proc_address("glResetHistogram"));
	glResetMinmax=reinterpret_cast<PFNGLRESETMINMAXPROC>(get_proc_address("glResetMinmax"));
	glTexImage3D=reinterpret_cast<PFNGLTEXIMAGE3DPROC>(get_proc_address("glTexImage3D"));
	glTexSubImage3D=reinterpret_cast<PFNGLTEXSUBIMAGE3DPROC>(get_proc_address("glTexSubImage3D"));
	glCopyTexSubImage3D=reinterpret_cast<PFNGLCOPYTEXSUBIMAGE3DPROC>(get_proc_address("glCopyTexSubImage3D"));
}

} // namespace GL
} // namespace Msp
