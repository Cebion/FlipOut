/* $Id: display.cpp 39 2009-03-23 11:07:37Z tdb $

This file is part of libmspgbase
Copyright © 2007-2008  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include <iostream>
#ifndef WIN32
#include <X11/Xlib.h>
#ifdef WITH_XF86VIDMODE
#include <X11/extensions/xf86vmode.h>
#endif
#endif
#include <msp/core/except.h>
#include <msp/strings/formatter.h>
#include <msp/strings/lexicalcast.h>
#include "display.h"
#include "window.h"
#include "display_priv.h"

using namespace std;

namespace {

bool error_flag=false;
std::string error_msg;

#ifndef WIN32
int x_error_handler(Display *display, XErrorEvent *event)
{
	char err[128];
	XGetErrorText(display, event->error_code, err, sizeof(err));

	string request_code=Msp::lexical_cast(static_cast<int>(event->request_code));
	char req[128];
	XGetErrorDatabaseText(display, "XRequest", request_code.c_str(), request_code.c_str(), req, sizeof(req));

	string msg=Msp::format("Request %s failed with %s [%08X]", req, err, event->resourceid);
	if(error_flag)
		cerr<<"Discarding error: "<<msg<<'\n';
	else
	{
		cerr<<msg<<'\n';
		error_msg=msg;
		error_flag=true;
	}

	return 0;
}
#endif

}

namespace Msp {
namespace Graphics {

Display::Display(const string &disp_name):
	priv(new Private)
{
#ifdef WIN32
	(void)disp_name;

	for(unsigned i=0;; ++i)
	{
		DEVMODE info;
		if(!EnumDisplaySettings(0, i, &info))
			break;

		VideoMode mode(info.dmPelsWidth, info.dmPelsHeight);
		mode.rate=info.dmDisplayFrequency;
		modes.push_back(mode);
	}
	
	DEVMODE info;
	if(EnumDisplaySettings(0, ENUM_CURRENT_SETTINGS, &info))
	{
		orig_mode=VideoMode(info.dmPelsWidth, info.dmPelsHeight);
		orig_mode.rate=info.dmDisplayFrequency;
	}
#else
	if(disp_name.empty())
		priv->display=XOpenDisplay(0);
	else
		priv->display=XOpenDisplay(disp_name.c_str());
	if(!priv->display)
		throw Exception("Couldn't open X display");

	XSetErrorHandler(x_error_handler);

#ifdef WITH_XF86VIDMODE
	int screen=DefaultScreen(priv->display);

	int nmodes;
	XF86VidModeModeInfo **infos;
	XF86VidModeGetAllModeLines(priv->display, screen, &nmodes, &infos);
	for(int i=0; i<nmodes; ++i)
	{
		XF86VidModeModeInfo &info=*infos[i];
	
		VideoMode mode(info.hdisplay, info.vdisplay);
		if(info.htotal && info.vtotal)
			mode.rate=info.dotclock/(info.htotal*info.vtotal);
		modes.push_back(mode);
	}

	XFree(infos);

	XF86VidModeModeLine modeline;
	int dotclock;
	XF86VidModeGetModeLine(priv->display, screen, &dotclock, &modeline);
	orig_mode=VideoMode(modeline.hdisplay, modeline.vdisplay);
	if(modeline.htotal && modeline.vtotal)
		orig_mode.rate=dotclock/(modeline.htotal*modeline.vtotal);
#endif
#endif
}

Display::~Display()
{
#ifndef WIN32
	XCloseDisplay(priv->display);
	delete priv;
#endif
}

void Display::add_window(Window &wnd)
{
	priv->windows[wnd.get_private().window]=&wnd;
}

void Display::remove_window(Window &wnd)
{
	priv->windows.erase(wnd.get_private().window);
}

void Display::set_mode(const VideoMode &mode)
{
#if defined(WIN32)
	DEVMODE info;
	info.dmSize=sizeof(DEVMODE);
	info.dmFields=DM_PELSWIDTH|DM_PELSHEIGHT;
	info.dmPelsWidth=mode.width;
	info.dmPelsHeight=mode.height;
	if(mode.rate)
	{
		info.dmFields|=DM_DISPLAYFREQUENCY;
		info.dmDisplayFrequency=mode.rate;
	}

	ChangeDisplaySettings(&info, CDS_FULLSCREEN);
#elif defined(WITH_XF86VIDMODE)
	int screen=DefaultScreen(priv->display);

	int nmodes;
	XF86VidModeModeInfo **infos;
	XF86VidModeGetAllModeLines(priv->display, screen, &nmodes, &infos);
	for(int i=0; i<nmodes; ++i)
	{
		XF86VidModeModeInfo &info=*infos[i];

		unsigned rate=0;
		if(info.htotal && info.vtotal)
			rate=info.dotclock/(info.htotal*info.vtotal);
		if(info.hdisplay==mode.width && info.vdisplay==mode.height && (mode.rate==0 || rate==mode.rate))
		{
			XF86VidModeSwitchToMode(priv->display, screen, &info);
			XF86VidModeSetViewPort(priv->display, screen, 0, 0);
			return;
		}
	}

	throw InvalidParameterValue("Requested mode not supported");
#else
	(void)mode;
	throw Exception("Video mode switching not supported");
#endif
}

void Display::tick()
{
	check_error();

	while(1)
	{
#ifdef WIN32
		MSG msg;
		if(PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
			DispatchMessage(&msg);
		else
			break;
#else
		int pending=XPending(priv->display);
		if(pending==0)
			break;

		for(; pending--;)
		{
			Window::Event event;
			XNextEvent(priv->display, &event.xevent);

			check_error();

			map<WindowHandle, Window *>::iterator j=priv->windows.find(event.xevent.xany.window);
			if(j!=priv->windows.end())
			{
				/* Filter keyboard autorepeat.  If this packet is a KeyRelease and
				the next one is a KeyPress with the exact same parameters, they
				indicate autorepeat and must be dropped. */
				if(event.xevent.type==KeyRelease && !j->second->get_keyboard_autorepeat() && pending>0)
				{
					XKeyEvent &kev=event.xevent.xkey;
					XEvent ev2;
					XPeekEvent(priv->display, &ev2);
					if(ev2.type==KeyPress)
					{
						XKeyEvent &kev2=ev2.xkey;
						if(kev2.window==kev.window && kev2.time==kev.time && kev2.keycode==kev.keycode)
						{
							XNextEvent(priv->display, &ev2);
							--pending;
							continue;
						}
					}
				}

				j->second->event(event);
			}
		}
#endif
	}
}

void Display::check_error()
{
	if(error_flag)
	{
		error_flag=false;
		throw Exception(error_msg);
	}
}

} // namespace Graphics
} // namespace Msp
