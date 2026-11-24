/* $Id: glcontext.cpp 30 2008-10-05 08:57:50Z tdb $

This file is part of libmspgbase
Copyright © 2007-2008  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include <vector>
#ifdef WIN32
#include <windows.h>
#endif
#ifdef WITH_OPENGL
#include <GL/gl.h>
#ifndef WIN32
#include <GL/glx.h>
#endif
#endif
#include <msp/core/application.h>
#include <msp/core/except.h>
#include "display.h"
#include "glcontext.h"
#include "window.h"
#include "display_priv.h"

namespace Msp {
namespace Graphics {

GLOptions::GLOptions():
	alpha(false),
	stencil(false),
	doublebuffer(true),
	multisample(0)
{ }


#ifdef WITH_OPENGL
#ifdef WIN32
typedef HGLRC Context;
#else
typedef GLXContext Context;
#endif

struct GLContext::Private
{
	Context context;
#ifndef WIN32
	// In X11, we need to create a window with the chosen visual
	WindowHandle subwnd;
#endif
};
#endif


GLContext::GLContext(Window &wnd, const GLOptions &opts):
	display(wnd.get_display()),
	window(wnd)
{
#ifdef WITH_OPENGL
	priv=new Private;

#ifdef WIN32
	HDC dc=GetDC(window.get_private().window);

	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(pfd));

	pfd.nSize=sizeof(pfd);
	pfd.nVersion=1;
	pfd.dwFlags=PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL;
	if(opts.doublebuffer)
		pfd.dwFlags|=PFD_DOUBLEBUFFER;
	pfd.iPixelType=PFD_TYPE_RGBA;
	if(opts.alpha)
		pfd.cAlphaBits=1;
	pfd.cDepthBits=1;
	if(opts.stencil)
		pfd.cStencilBits=1;

	int pf_index=ChoosePixelFormat(dc, &pfd);
	if(!pf_index)
		throw Exception("Couldn't find a suitable pixel format");
	SetPixelFormat(dc, pf_index, &pfd);

	priv->context=wglCreateContext(dc);
	wglMakeCurrent(dc, priv->context);

	ReleaseDC(window.get_private().window, dc);
#else
	std::vector<int> attribs;
	
	attribs.push_back(GLX_RGBA);
	attribs.push_back(GLX_DEPTH_SIZE);
	attribs.push_back(1);
	
	if(opts.alpha)
	{
		attribs.push_back(GLX_ALPHA_SIZE);
		attribs.push_back(1);
	}
	
	if(opts.stencil)
	{
		attribs.push_back(GLX_STENCIL_SIZE);
		attribs.push_back(1);
	}
	
	if(opts.doublebuffer)
		attribs.push_back(GLX_DOUBLEBUFFER);
	
	if(opts.multisample>0)
	{
		attribs.push_back(GLX_SAMPLE_BUFFERS_ARB);
		attribs.push_back(opts.multisample);
	}
	
	attribs.push_back(0);

	::Display *dpy=display.get_private().display;

	XVisualInfo *vi=glXChooseVisual(dpy, DefaultScreen(dpy), &attribs.front());
	if(!vi)
		throw Exception("Couldn't find a suitable GLX visual");
	priv->context=glXCreateContext(dpy, vi, 0, true);

	XSetWindowAttributes attr;
	attr.colormap=XCreateColormap(dpy, DefaultRootWindow(dpy), vi->visual, AllocNone);

	priv->subwnd=XCreateWindow(dpy, window.get_private().window, 0, 0, window.get_width(), window.get_height(), 0, vi->depth, InputOutput, vi->visual, CWColormap, &attr);
	XMapWindow(dpy, priv->subwnd);

	XFree(vi);

	glXMakeCurrent(dpy, priv->subwnd, priv->context);
#endif
#else
	(void)wnd;
	(void)opts;
	throw Exception("OpenGL support not compiled in");
#endif

	window.signal_resize.connect(sigc::mem_fun(this, &GLContext::window_resized));
}

GLContext::~GLContext()
{
#ifdef WITH_OPENGL
#ifdef WIN32
	wglMakeCurrent(0, 0);
	wglDeleteContext(priv->context);
#else
	::Display *dpy=display.get_private().display;

	glXMakeCurrent(dpy, 0, 0);
	glXDestroyContext(dpy, priv->context);
	XDestroyWindow(dpy, priv->subwnd);
#endif
	delete priv;
#endif
}

void GLContext::swap_buffers()
{
#ifdef WITH_OPENGL
#ifdef WIN32
	HDC dc=GetDC(window.get_private().window);
	SwapBuffers(dc);
	ReleaseDC(window.get_private().window, dc);
#else
	glXSwapBuffers(display.get_private().display, priv->subwnd);
#endif
#endif
}

void GLContext::window_resized(unsigned w, unsigned h)
{
#ifdef WITH_OPENGL
#ifndef WIN32
	XMoveResizeWindow(display.get_private().display, priv->subwnd, 0, 0, w, h);
#endif
	glViewport(0, 0, w, h);
#else
	(void)w;
	(void)h;
#endif
}

} // namespace Graphics
} // namespace Msp
