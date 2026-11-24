/* $Id: window.h 41 2009-08-03 12:26:54Z tdb $

This file is part of libmspgbase
Copyright © 2007-2008  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_GBASE_WINDOW_H_
#define MSP_GBASE_WINDOW_H_

#include <string>
#include <sigc++/signal.h>

namespace Msp {
namespace Graphics {

class Display;

struct WindowOptions
{
	unsigned width;
	unsigned height;
	bool fullscreen;
	bool resizable;

	WindowOptions();
};

class Window
{
public:
	struct Private;
	struct Event;

	sigc::signal<void, int, int, unsigned, unsigned> signal_button_press;
	sigc::signal<void, int, int, unsigned, unsigned> signal_button_release;
	sigc::signal<void, int, int> signal_pointer_motion;
	sigc::signal<void, unsigned, unsigned, unsigned> signal_key_press;
	sigc::signal<void, unsigned, unsigned> signal_key_release;
	sigc::signal<void, unsigned, unsigned> signal_resize;
	sigc::signal<void> signal_close;

protected:
	Display &display;
	WindowOptions options;
	bool visible;
	bool kbd_autorepeat;
	bool resizing;
	Private *priv;

public:
	Window(Display &, unsigned w, unsigned h, bool fs=false);
	Window(Display &, const WindowOptions &);
private:
	void init();
public:
	~Window();

	void set_title(const std::string &);
	void reconfigure(const WindowOptions &);
	void set_keyboard_autorepeat(bool);
	bool get_keyboard_autorepeat() const { return kbd_autorepeat; }
	void show_cursor(bool);
	void warp_pointer(int, int);

	Display &get_display() const { return display; }
	const WindowOptions &get_options() const { return options; }
	unsigned get_width() const  { return options.width; }
	unsigned get_height() const { return options.height; }
	const Private &get_private() const { return *priv; }

	void show();
	void hide();

	bool event(const Event &evnt);
};

} // namespace Graphics
} // namespace Msp

#endif
