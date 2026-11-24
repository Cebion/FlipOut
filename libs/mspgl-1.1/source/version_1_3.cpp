#include "extension.h"
#include "version_1_3.h"

namespace Msp {
namespace GL {

PFNGLACTIVETEXTUREPROC glActiveTexture=0;
PFNGLCLIENTACTIVETEXTUREPROC glClientActiveTexture=0;
PFNGLMULTITEXCOORD1DPROC glMultiTexCoord1d=0;
PFNGLMULTITEXCOORD1DVPROC glMultiTexCoord1dv=0;
PFNGLMULTITEXCOORD1FPROC glMultiTexCoord1f=0;
PFNGLMULTITEXCOORD1FVPROC glMultiTexCoord1fv=0;
PFNGLMULTITEXCOORD1IPROC glMultiTexCoord1i=0;
PFNGLMULTITEXCOORD1IVPROC glMultiTexCoord1iv=0;
PFNGLMULTITEXCOORD1SPROC glMultiTexCoord1s=0;
PFNGLMULTITEXCOORD1SVPROC glMultiTexCoord1sv=0;
PFNGLMULTITEXCOORD2DPROC glMultiTexCoord2d=0;
PFNGLMULTITEXCOORD2DVPROC glMultiTexCoord2dv=0;
PFNGLMULTITEXCOORD2FPROC glMultiTexCoord2f=0;
PFNGLMULTITEXCOORD2FVPROC glMultiTexCoord2fv=0;
PFNGLMULTITEXCOORD2IPROC glMultiTexCoord2i=0;
PFNGLMULTITEXCOORD2IVPROC glMultiTexCoord2iv=0;
PFNGLMULTITEXCOORD2SPROC glMultiTexCoord2s=0;
PFNGLMULTITEXCOORD2SVPROC glMultiTexCoord2sv=0;
PFNGLMULTITEXCOORD3DPROC glMultiTexCoord3d=0;
PFNGLMULTITEXCOORD3DVPROC glMultiTexCoord3dv=0;
PFNGLMULTITEXCOORD3FPROC glMultiTexCoord3f=0;
PFNGLMULTITEXCOORD3FVPROC glMultiTexCoord3fv=0;
PFNGLMULTITEXCOORD3IPROC glMultiTexCoord3i=0;
PFNGLMULTITEXCOORD3IVPROC glMultiTexCoord3iv=0;
PFNGLMULTITEXCOORD3SPROC glMultiTexCoord3s=0;
PFNGLMULTITEXCOORD3SVPROC glMultiTexCoord3sv=0;
PFNGLMULTITEXCOORD4DPROC glMultiTexCoord4d=0;
PFNGLMULTITEXCOORD4DVPROC glMultiTexCoord4dv=0;
PFNGLMULTITEXCOORD4FPROC glMultiTexCoord4f=0;
PFNGLMULTITEXCOORD4FVPROC glMultiTexCoord4fv=0;
PFNGLMULTITEXCOORD4IPROC glMultiTexCoord4i=0;
PFNGLMULTITEXCOORD4IVPROC glMultiTexCoord4iv=0;
PFNGLMULTITEXCOORD4SPROC glMultiTexCoord4s=0;
PFNGLMULTITEXCOORD4SVPROC glMultiTexCoord4sv=0;
PFNGLLOADTRANSPOSEMATRIXFPROC glLoadTransposeMatrixf=0;
PFNGLLOADTRANSPOSEMATRIXDPROC glLoadTransposeMatrixd=0;
PFNGLMULTTRANSPOSEMATRIXFPROC glMultTransposeMatrixf=0;
PFNGLMULTTRANSPOSEMATRIXDPROC glMultTransposeMatrixd=0;
PFNGLSAMPLECOVERAGEPROC glSampleCoverage=0;
PFNGLCOMPRESSEDTEXIMAGE3DPROC glCompressedTexImage3D=0;
PFNGLCOMPRESSEDTEXIMAGE2DPROC glCompressedTexImage2D=0;
PFNGLCOMPRESSEDTEXIMAGE1DPROC glCompressedTexImage1D=0;
PFNGLCOMPRESSEDTEXSUBIMAGE3DPROC glCompressedTexSubImage3D=0;
PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC glCompressedTexSubImage2D=0;
PFNGLCOMPRESSEDTEXSUBIMAGE1DPROC glCompressedTexSubImage1D=0;
PFNGLGETCOMPRESSEDTEXIMAGEPROC glGetCompressedTexImage=0;

void init_version_1_3()
{
	glActiveTexture=reinterpret_cast<PFNGLACTIVETEXTUREPROC>(get_proc_address("glActiveTexture"));
	glClientActiveTexture=reinterpret_cast<PFNGLCLIENTACTIVETEXTUREPROC>(get_proc_address("glClientActiveTexture"));
	glMultiTexCoord1d=reinterpret_cast<PFNGLMULTITEXCOORD1DPROC>(get_proc_address("glMultiTexCoord1d"));
	glMultiTexCoord1dv=reinterpret_cast<PFNGLMULTITEXCOORD1DVPROC>(get_proc_address("glMultiTexCoord1dv"));
	glMultiTexCoord1f=reinterpret_cast<PFNGLMULTITEXCOORD1FPROC>(get_proc_address("glMultiTexCoord1f"));
	glMultiTexCoord1fv=reinterpret_cast<PFNGLMULTITEXCOORD1FVPROC>(get_proc_address("glMultiTexCoord1fv"));
	glMultiTexCoord1i=reinterpret_cast<PFNGLMULTITEXCOORD1IPROC>(get_proc_address("glMultiTexCoord1i"));
	glMultiTexCoord1iv=reinterpret_cast<PFNGLMULTITEXCOORD1IVPROC>(get_proc_address("glMultiTexCoord1iv"));
	glMultiTexCoord1s=reinterpret_cast<PFNGLMULTITEXCOORD1SPROC>(get_proc_address("glMultiTexCoord1s"));
	glMultiTexCoord1sv=reinterpret_cast<PFNGLMULTITEXCOORD1SVPROC>(get_proc_address("glMultiTexCoord1sv"));
	glMultiTexCoord2d=reinterpret_cast<PFNGLMULTITEXCOORD2DPROC>(get_proc_address("glMultiTexCoord2d"));
	glMultiTexCoord2dv=reinterpret_cast<PFNGLMULTITEXCOORD2DVPROC>(get_proc_address("glMultiTexCoord2dv"));
	glMultiTexCoord2f=reinterpret_cast<PFNGLMULTITEXCOORD2FPROC>(get_proc_address("glMultiTexCoord2f"));
	glMultiTexCoord2fv=reinterpret_cast<PFNGLMULTITEXCOORD2FVPROC>(get_proc_address("glMultiTexCoord2fv"));
	glMultiTexCoord2i=reinterpret_cast<PFNGLMULTITEXCOORD2IPROC>(get_proc_address("glMultiTexCoord2i"));
	glMultiTexCoord2iv=reinterpret_cast<PFNGLMULTITEXCOORD2IVPROC>(get_proc_address("glMultiTexCoord2iv"));
	glMultiTexCoord2s=reinterpret_cast<PFNGLMULTITEXCOORD2SPROC>(get_proc_address("glMultiTexCoord2s"));
	glMultiTexCoord2sv=reinterpret_cast<PFNGLMULTITEXCOORD2SVPROC>(get_proc_address("glMultiTexCoord2sv"));
	glMultiTexCoord3d=reinterpret_cast<PFNGLMULTITEXCOORD3DPROC>(get_proc_address("glMultiTexCoord3d"));
	glMultiTexCoord3dv=reinterpret_cast<PFNGLMULTITEXCOORD3DVPROC>(get_proc_address("glMultiTexCoord3dv"));
	glMultiTexCoord3f=reinterpret_cast<PFNGLMULTITEXCOORD3FPROC>(get_proc_address("glMultiTexCoord3f"));
	glMultiTexCoord3fv=reinterpret_cast<PFNGLMULTITEXCOORD3FVPROC>(get_proc_address("glMultiTexCoord3fv"));
	glMultiTexCoord3i=reinterpret_cast<PFNGLMULTITEXCOORD3IPROC>(get_proc_address("glMultiTexCoord3i"));
	glMultiTexCoord3iv=reinterpret_cast<PFNGLMULTITEXCOORD3IVPROC>(get_proc_address("glMultiTexCoord3iv"));
	glMultiTexCoord3s=reinterpret_cast<PFNGLMULTITEXCOORD3SPROC>(get_proc_address("glMultiTexCoord3s"));
	glMultiTexCoord3sv=reinterpret_cast<PFNGLMULTITEXCOORD3SVPROC>(get_proc_address("glMultiTexCoord3sv"));
	glMultiTexCoord4d=reinterpret_cast<PFNGLMULTITEXCOORD4DPROC>(get_proc_address("glMultiTexCoord4d"));
	glMultiTexCoord4dv=reinterpret_cast<PFNGLMULTITEXCOORD4DVPROC>(get_proc_address("glMultiTexCoord4dv"));
	glMultiTexCoord4f=reinterpret_cast<PFNGLMULTITEXCOORD4FPROC>(get_proc_address("glMultiTexCoord4f"));
	glMultiTexCoord4fv=reinterpret_cast<PFNGLMULTITEXCOORD4FVPROC>(get_proc_address("glMultiTexCoord4fv"));
	glMultiTexCoord4i=reinterpret_cast<PFNGLMULTITEXCOORD4IPROC>(get_proc_address("glMultiTexCoord4i"));
	glMultiTexCoord4iv=reinterpret_cast<PFNGLMULTITEXCOORD4IVPROC>(get_proc_address("glMultiTexCoord4iv"));
	glMultiTexCoord4s=reinterpret_cast<PFNGLMULTITEXCOORD4SPROC>(get_proc_address("glMultiTexCoord4s"));
	glMultiTexCoord4sv=reinterpret_cast<PFNGLMULTITEXCOORD4SVPROC>(get_proc_address("glMultiTexCoord4sv"));
	glLoadTransposeMatrixf=reinterpret_cast<PFNGLLOADTRANSPOSEMATRIXFPROC>(get_proc_address("glLoadTransposeMatrixf"));
	glLoadTransposeMatrixd=reinterpret_cast<PFNGLLOADTRANSPOSEMATRIXDPROC>(get_proc_address("glLoadTransposeMatrixd"));
	glMultTransposeMatrixf=reinterpret_cast<PFNGLMULTTRANSPOSEMATRIXFPROC>(get_proc_address("glMultTransposeMatrixf"));
	glMultTransposeMatrixd=reinterpret_cast<PFNGLMULTTRANSPOSEMATRIXDPROC>(get_proc_address("glMultTransposeMatrixd"));
	glSampleCoverage=reinterpret_cast<PFNGLSAMPLECOVERAGEPROC>(get_proc_address("glSampleCoverage"));
	glCompressedTexImage3D=reinterpret_cast<PFNGLCOMPRESSEDTEXIMAGE3DPROC>(get_proc_address("glCompressedTexImage3D"));
	glCompressedTexImage2D=reinterpret_cast<PFNGLCOMPRESSEDTEXIMAGE2DPROC>(get_proc_address("glCompressedTexImage2D"));
	glCompressedTexImage1D=reinterpret_cast<PFNGLCOMPRESSEDTEXIMAGE1DPROC>(get_proc_address("glCompressedTexImage1D"));
	glCompressedTexSubImage3D=reinterpret_cast<PFNGLCOMPRESSEDTEXSUBIMAGE3DPROC>(get_proc_address("glCompressedTexSubImage3D"));
	glCompressedTexSubImage2D=reinterpret_cast<PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC>(get_proc_address("glCompressedTexSubImage2D"));
	glCompressedTexSubImage1D=reinterpret_cast<PFNGLCOMPRESSEDTEXSUBIMAGE1DPROC>(get_proc_address("glCompressedTexSubImage1D"));
	glGetCompressedTexImage=reinterpret_cast<PFNGLGETCOMPRESSEDTEXIMAGEPROC>(get_proc_address("glGetCompressedTexImage"));
}

} // namespace GL
} // namespace Msp
