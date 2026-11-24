/* $Id: window.cpp 41 2009-08-03 12:26:54Z tdb $

This file is part of libmspgbase
Copyright © 2007-2008  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include <vector>
#ifndef WIN32
#include <X11/Xatom.h>
#include <X11/Xutil.h>
#ifdef WITH_XF86VIDMODE
#include <X11/extensions/xf86vmode.h>
#endif
#else
#include <windowsx.h>
#endif
#include <msp/core/application.h>
#include <msp/core/except.h>
#include "display.h"
#include "window.h"
#include "display_priv.h"

using namespace std;

namespace {

#ifdef WIN32
LRESULT CALLBACK wndproc_(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	if(msg==WM_CREATE)
	{
		CREATESTRUCT *cs=reinterpret_cast<CREATESTRUCT *>(lparam);
		SetWindowLong(hwnd, 0, reinterpret_cast<LONG>(cs->lpCreateParams));
	}
	else
	{
		Msp::Graphics::Window *wnd=reinterpret_cast<Msp::Graphics::Window *>(GetWindowLong(hwnd, 0));
		Msp::Graphics::Window::Event ev;
		ev.msg=msg;
		ev.wparam=wparam;
		ev.lparam=lparam;
		if(wnd && wnd->event(ev))
			return 0;
	}

	return DefWindowProc(hwnd, msg, wparam, lparam);
}
#else
Bool match_event_type(Display *, XEvent *event, XPointer arg)
{
	return event->type==reinterpret_cast<int>(arg);
}
#endif

}

namespace Msp {
namespace Graphics {

WindowOptions::WindowOptions():
	width(640),
	height(480),
	fullscreen(false),
	resizable(false)
{ }


Window::Window(Display &dpy, unsigned w, unsigned h, bool fs):
	display(dpy)
{
	options.width=w;
	options.height=h;
	options.fullscreen=fs;

	init();
}

Window::Window(Display &dpy, const WindowOptions &opts):
	display(dpy),
	options(opts)
{
	init();
}

void Window::init()
{
	visible=false;
	kbd_autorepeat=true;
	resizing=false;
	priv=new Private;

#ifdef WIN32
	static bool wndclass_created=false;

	if(!wndclass_created)
	{
		WNDCLASSEX wndcl;

		wndcl.cbSize=sizeof(WNDCLASSEX);
		wndcl.style=0;
		wndcl.lpfnWndProc=&wndproc_;
		wndcl.cbClsExtra=0;
		wndcl.cbWndExtra=sizeof(Window *);
		wndcl.hInstance=reinterpret_cast<HINSTANCE>(Application::get_data());
		wndcl.hIcon=0;
		wndcl.hCursor=LoadCursor(0, IDC_ARROW);
		wndcl.hbrBackground=0;
		wndcl.lpszMenuName=0;
		wndcl.lpszClassName="mspgbase";
		wndcl.hIconSm=0;

		if(!RegisterClassEx(&wndcl))
			throw Exception("Couldn't register window class");

		wndclass_created=true;
	}

	RECT rect;
	SetRect(&rect, 0, 0, options.width, options.height);

	int style=(options.fullscreen ? WS_POPUP : WS_OVERLAPPEDWINDOW);
	if(!options.resizable)
		style&=~WS_THICKFRAME;
	int exstyle=(options.fullscreen ? WS_EX_APPWINDOW : WS_EX_OVERLAPPEDWINDOW);
	AdjustWindowRectEx(&rect, style, false, exstyle);

	priv->window=CreateWindowEx(exstyle,
		"mspgbase",
		"Window",
		style,
		CW_USEDEFAULT, CW_USEDEFAULT,
		rect.right-rect.left, rect.bottom-rect.top,
		0,
		0,
		reinterpret_cast<HINSTANCE>(Application::get_data()),
		this);
	if(!priv->window)
		throw Exception("CreateWindowEx failed");

#else
	::Display *dpy=display.get_private().display;

	priv->wm_delete_window=XInternAtom(dpy, "WM_DELETE_WINDOW", true);
	priv->invisible_cursor=0;

	XSetWindowAttributes attr;
	attr.override_redirect=options.fullscreen;
	attr.event_mask=ButtonPressMask|ButtonReleaseMask|PointerMotionMask|KeyPressMask|KeyReleaseMask|StructureNotifyMask|EnterWindowMask;

	priv->window=XCreateWindow(dpy,
		DefaultRootWindow(dpy),
		0, 0,
		options.width, options.height,
		0,
		CopyFromParent,
		InputOutput,
		CopyFromParent,
		CWOverrideRedirect|CWEventMask, &attr);

	XSetWMProtocols(dpy, priv->window, &priv->wm_delete_window, 1);

	if(!options.resizable)
	{
		XSizeHints hints;
		hints.flags=PMinSize|PMaxSize;
		hints.min_width=hints.max_width=options.width;
		hints.min_height=hints.max_height=options.height;
		XSetWMNormalHints(dpy, priv->window, &hints);
	}

#endif

	display.add_window(*this);
	display.check_error();
}

Window::~Window()
{
	if(priv->window)
#ifdef WIN32
		CloseWindow(priv->window);
#else
		XDestroyWindow(display.get_private().display, priv->window);

	if(priv->invisible_cursor)
		XFreeCursor(display.get_private().display, priv->invisible_cursor);
#endif

	display.remove_window(*this);

	if(options.fullscreen)
		display.restore_mode();

	delete priv;
}

void Window::set_title(const string &title)
{
#ifdef WIN32
	SetWindowText(priv->window, title.c_str());
#else
	vector<unsigned char> buf(title.begin(), title.end());
	XTextProperty prop;
	prop.value=&buf[0];
	prop.encoding=XA_STRING;
	prop.format=8;
	prop.nitems=title.size();
	XSetWMName(display.get_private().display, priv->window, &prop);
	display.check_error();
#endif
}

void Window::reconfigure(const WindowOptions &opts)
{
	bool fullscreen_changed=(opts.fullscreen!=options.fullscreen);
	resizing=(opts.width!=options.width || opts.height!=options.height);

	options=opts;

#ifdef WIN32
	RECT rect;
	SetRect(&rect, 0, 0, options.width, options.height);

	int style=(options.fullscreen ? WS_POPUP : WS_OVERLAPPEDWINDOW);
	if(!options.resizable)
		style&=~WS_THICKFRAME;
	int exstyle=(options.fullscreen ? WS_EX_APPWINDOW : WS_EX_OVERLAPPEDWINDOW);
	AdjustWindowRectEx(&rect, style, false, exstyle);

	if(fullscreen_changed)
	{
		hide();
		SetWindowLong(priv->window, GWL_EXSTYLE, exstyle);
		SetWindowLong(priv->window, GWL_STYLE, style);
		show();
	}

	if(options.fullscreen)
		SetWindowPos(priv->window, 0, 0, 0, rect.right-rect.left, rect.bottom-rect.top, SWP_NOZORDER);
	else
		SetWindowPos(priv->window, 0, 0, 0, rect.right-rect.left, rect.bottom-rect.top, SWP_NOMOVE|SWP_NOZORDER);
#else
	::Display *dpy=display.get_private().display;

	bool was_visible=visible;
	if(fullscreen_changed)
	{
		if(was_visible)
		{
			hide();

			// Wait for the window to be unmapped.  This makes window managers happy.
			XEvent ev;
			XPeekIfEvent(dpy, &ev, match_event_type, reinterpret_cast<XPointer>(UnmapNotify));
		}

		XSetWindowAttributes attr;
		attr.override_redirect=options.fullscreen;
		XChangeWindowAttributes(dpy, priv->window, CWOverrideRedirect, &attr);
	}

	XSizeHints hints;
	if(options.resizable)
		hints.flags=0;
	else
	{
		hints.flags=PMinSize|PMaxSize;
		hints.min_width=hints.max_width=options.width;
		hints.min_height=hints.max_height=options.height;
	}
	XSetWMNormalHints(dpy, priv->window, &hints);

	if(options.fullscreen)
		XMoveResizeWindow(dpy, priv->window, 0, 0, options.width, options.height);
	else
		XResizeWindow(dpy, priv->window, options.width, options.height);

	if(fullscreen_changed)
	{
		if(was_visible)
			show();
	}
#endif

	if(visible)
	{
		if(options.fullscreen)
			display.set_mode(VideoMode(options.width, options.height));
		else if(fullscreen_changed)
			display.restore_mode();
	}
}

void Window::set_keyboard_autorepeat(bool r)
{
	kbd_autorepeat=r;
}

void Window::show_cursor(bool s)
{
#ifdef WIN32
	ShowCursor(s);
#else
	::Display *dpy=display.get_private().display;

	if(s)
		XUndefineCursor(dpy, priv->window);
	else
	{
		if(!priv->invisible_cursor)
		{
			int screen=DefaultScreen(dpy);

			Pixmap pm=XCreatePixmap(dpy, priv->window, 1, 1, 1);
			GC gc=XCreateGC(dpy, pm, 0, 0);
			XSetFunction(dpy, gc, GXclear);
			XDrawPoint(dpy, pm, gc, 0, 0);
			XFreeGC(dpy, gc);

			XColor black;
			black.pixel=BlackPixel(dpy, screen);
			XQueryColor(dpy, DefaultColormap(dpy, screen), &black);

			priv->invisible_cursor=XCreatePixmapCursor(dpy, pm, pm, &black, &black, 0, 0);

			XFreePixmap(dpy, pm);
		}
		XDefineCursor(dpy, priv->window, priv->invisible_cursor);
	}
#endif
}

void Window::warp_pointer(int x, int y)
{
#ifndef WIN32
	XWarpPointer(display.get_private().display, None, priv->window, 0, 0, 0, 0, x, y);
#else
	(void)x;
	(void)y;
#endif
}

void Window::show()
{
#ifdef WIN32
	ShowWindow(priv->window, SW_SHOWNORMAL);
#else
	XMapRaised(display.get_private().display, priv->window);
#endif
	visible=true;

	if(options.fullscreen)
	{
		display.set_mode(VideoMode(options.width, options.height));
#ifndef WIN32
		XWarpPointer(display.get_private().display, None, priv->window, 0, 0, 0, 0, options.width/2, options.height/2);
#endif
	}
}

void Window::hide()
{
#ifdef WIN32
	ShowWindow(priv->window, SW_HIDE);
#else
	XUnmapWindow(display.get_private().display, priv->window);
#endif
	visible=false;

	if(options.fullscreen)
		display.restore_mode();
}

bool Window::event(const Event &evnt)
{
#ifdef WIN32
	WPARAM wp=evnt.wparam;
	LPARAM lp=evnt.lparam;
	switch(evnt.msg)
	{
	case WM_KEYDOWN:
		signal_key_press.emit(wp, 0, wp);
		break;
	case WM_KEYUP:
		signal_key_release.emit(wp, 0);
		break;
	case WM_LBUTTONDOWN:
		signal_button_press.emit(GET_X_LPARAM(lp), GET_Y_LPARAM(lp), 1, 0);
		break;
	case WM_LBUTTONUP:
		signal_button_release.emit(GET_X_LPARAM(lp), GET_Y_LPARAM(lp), 1, 0);
		break;
	case WM_MBUTTONDOWN:
		signal_button_press.emit(GET_X_LPARAM(lp), GET_Y_LPARAM(lp), 2, 0);
		break;
	case WM_MBUTTONUP:
		signal_button_release.emit(GET_X_LPARAM(lp), GET_Y_LPARAM(lp), 2, 0);
		break;
	case WM_RBUTTONDOWN:
		signal_button_press.emit(GET_X_LPARAM(lp), GET_Y_LPARAM(lp), 3, 0);
		break;
	case WM_RBUTTONUP:
		signal_button_release.emit(GET_X_LPARAM(lp), GET_Y_LPARAM(lp), 3, 0);
		break;
	case WM_MOUSEWHEEL:
		{
			unsigned btn = (HIWORD(wp)&0x8000) ? 5 : 4;
			signal_button_press.emit(GET_X_LPARAM(lp), GET_Y_LPARAM(lp), btn, 0);
			signal_button_release.emit(GET_X_LPARAM(lp), GET_Y_LPARAM(lp), btn, 0);
		}
		break;
	case WM_MOUSEMOVE:
		signal_pointer_motion.emit(GET_X_LPARAM(lp), GET_Y_LPARAM(lp));
		break;
	case WM_SIZE:
		options.width=LOWORD(lp);
		options.height=HIWORD(lp);
		signal_resize.emit(options.width, options.height);
		break;
	case WM_CLOSE:
		signal_close.emit();
		break;
	default:
		return false;
	}
#else
	const XEvent &ev=evnt.xevent;
	switch(ev.type)
	{
	case ButtonPress:
		signal_button_press.emit(ev.xbutton.x, ev.xbutton.y, ev.xbutton.button, ev.xbutton.state);
		break;
	case ButtonRelease:
		signal_button_release.emit(ev.xbutton.x, ev.xbutton.y, ev.xbutton.button, ev.xbutton.state);
		break;
	case MotionNotify:
		signal_pointer_motion.emit(ev.xmotion.x, ev.xmotion.y);
		break;
	case KeyPress:
		{
			char buf[16];
			XLookupString(const_cast<XKeyEvent *>(&ev.xkey), buf, sizeof(buf), 0, 0);
			// XXX Handle the result according to locale
			signal_key_press.emit(XKeycodeToKeysym(display.get_private().display, ev.xkey.keycode, 0), ev.xkey.state, buf[0]);
		}
		break;
	case KeyRelease:
		signal_key_release.emit(XKeycodeToKeysym(display.get_private().display, ev.xkey.keycode, 0), ev.xkey.state);
		break;
	case ConfigureNotify:
		if((ev.xconfigure.width==static_cast<int>(options.width) && ev.xconfigure.height==static_cast<int>(options.height)) == resizing)
		{
			options.width=ev.xconfigure.width;
			options.height=ev.xconfigure.height;
			resizing=false;
			signal_resize.emit(options.width, options.height);
		}
		if(options.fullscreen)
		{
			::Display *dpy=display.get_private().display;
			int screen=DefaultScreen(dpy);
			XF86VidModeSetViewPort(dpy, screen, ev.xconfigure.x, ev.xconfigure.y);
		}
		break;
	case ClientMessage:
		if(ev.xclient.data.l[0]==static_cast<long>(priv->wm_delete_window))
			signal_close.emit();
		break;
	case EnterNotify:
		if(options.fullscreen)
			XSetInputFocus(display.get_private().display, priv->window, RevertToParent, CurrentTime);
		break;
	case MapNotify:
		if(options.fullscreen)
			XGrabPointer(display.get_private().display, priv->window, true, None, GrabModeAsync, GrabModeAsync, priv->window, None, CurrentTime);
		break;
	default:
		return false;
	}
#endif
	return true;
}

} // namespace Graphics
} // namespace Msp
