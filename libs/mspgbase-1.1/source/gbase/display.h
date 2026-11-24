/* $Id: display.h 30 2008-10-05 08:57:50Z tdb $

This file is part of libmspgbase
Copyright © 2007-2008  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_GBASE_DISPLAY_H_
#define MSP_GBASE_DISPLAY_H_

#include <list>
#include <map>
#include <string>

namespace Msp {
namespace Graphics {

class Window;

struct VideoMode
{
	unsigned width;
	unsigned height;
	unsigned rate;

	VideoMode(): width(0), height(0), rate(0) { }
	VideoMode(unsigned w, unsigned h): width(w), height(h), rate(0) { }
};

class Display
{
public:
	struct Private;

private:
	std::list<VideoMode> modes;
	VideoMode orig_mode;
	Private *priv;

public:
	Display(const std::string &disp_name=std::string());
	~Display();

	const Private &get_private() const { return *priv; }

	void add_window(Window &);
	void remove_window(Window &);

	const std::list<VideoMode> &get_modes() const { return modes; }
	void set_mode(const VideoMode &);
	void restore_mode() { set_mode(orig_mode); }

	void tick();
	void check_error();
};

} // namespace Graphics
} // namespace Msp

#endif
