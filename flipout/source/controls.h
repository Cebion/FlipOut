/* $Id$

This file is part of flipout
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef CONTROLS_H_
#define CONTROLS_H_

#include <map>
#include <string>
#include <msp/input/binarycontrol.h>
#include <msp/input/smoothcontrol.h>

struct Controls
{
	Msp::Input::SmoothControl flip_left;
	Msp::Input::SmoothControl flip_right;
	Msp::Input::SmoothControl move_left;
	Msp::Input::SmoothControl move_right;
	Msp::Input::BinaryControl turbo;
	Msp::Input::BinaryControl pause;
	Msp::Input::BinaryControl slow_motion;
	Msp::Input::BinaryControl exit;

	std::map<std::string, Msp::Input::Control *> by_name;

	Controls();
	Msp::Input::Control &get(const std::string &) const;
};

#endif
