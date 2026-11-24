/* $Id: drawcontext.cpp 30 2008-10-05 08:57:50Z tdb $

This file is part of libmspgbase
Copyright © 2008  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef WIN32
#include <sys/ipc.h>
#include <sys/shm.h>
#include <X11/Xlib.h>
#include <X11/extensions/XShm.h>
#include <X11/Xutil.h>
#endif
#include <msp/core/except.h>
#include "display.h"
#include "drawcontext.h"
#include "window.h"
#include "display_priv.h"

namespace Msp {
namespace Graphics {

struct DrawContext::Private
{
#ifndef WIN32
	XImage *image;
	bool use_shm;
	XShmSegmentInfo shminfo;
#endif
};

DrawContext::DrawContext(Window &w):
	display(w.get_display()),
	window(w)
{
#ifdef WIN32
	throw Exception("DrawContext not supported on win32 yet");
#else
	priv=new Private;

	::Display *dpy=display.get_private().display;

	priv->use_shm=XShmQueryExtension(dpy);

	XWindowAttributes wa;
	XGetWindowAttributes(dpy, window.get_private().window, &wa);

	if(priv->use_shm)
	{
		priv->image=XShmCreateImage(dpy, wa.visual, wa.depth, ZPixmap, 0, &priv->shminfo, wa.width, wa.height);
		if(!priv->image)
			throw Exception("Could not create shared memory XImage");

		priv->shminfo.shmid=shmget(IPC_PRIVATE, priv->image->bytes_per_line*priv->image->height, IPC_CREAT|0666);
		priv->shminfo.shmaddr=priv->image->data=reinterpret_cast<char *>(shmat(priv->shminfo.shmid, 0, 0));
		priv->shminfo.readOnly=false;

		XShmAttach(dpy, &priv->shminfo);

		XSync(dpy, false);
		display.check_error();
	}
	else
	{
		priv->image=XCreateImage(dpy, wa.visual, wa.depth, ZPixmap, 0, 0, wa.width, wa.height, 8, 0);
		if(!priv->image)
			throw Exception("Could not create XImage");
		priv->image->data=new char[priv->image->bytes_per_line*priv->image->height];
	}
#endif
}

DrawContext::~DrawContext()
{
#ifndef WIN32
	if(priv->use_shm)
	{
		XShmDetach(display.get_private().display, &priv->shminfo);
		shmdt(priv->shminfo.shmaddr);
		shmctl(priv->shminfo.shmid, IPC_RMID, 0);
	}

	XDestroyImage(priv->image);
#endif

	delete priv;
}

unsigned DrawContext::get_depth() const
{
#ifdef WIN32
	return 0;
#else
	return priv->image->bits_per_pixel;
#endif
}

unsigned char *DrawContext::get_data()
{
#ifdef WIN32
	return 0;
#else
	return reinterpret_cast<unsigned char *>(priv->image->data);
#endif
}

void DrawContext::update()
{
#ifndef WIN32
	::Display *dpy=display.get_private().display;
	WindowHandle wnd=window.get_private().window;

	GC gc=XCreateGC(dpy, wnd, 0, 0);

	if(priv->use_shm)
		XShmPutImage(dpy, wnd, gc, priv->image, 0, 0, 0, 0, priv->image->width, priv->image->height, false);
	else
		XPutImage(dpy, wnd, gc, priv->image, 0, 0, 0, 0, priv->image->width, priv->image->height);

	XFreeGC(dpy, gc);
#endif
}

} // namespace Graphics
} // namespace Msp
