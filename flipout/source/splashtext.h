/* $Id$

This file is part of flipout
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef SPLASHTEXT_H_
#define SPLASHTEXT_H_

#include <msp/gl/color.h>
#include <msp/gl/font.h>
#include <msp/time/timedelta.h>

class SplashText
{
private:
	const Msp::GL::Font &font;
	std::string text;
	float zoom;
	Msp::GL::Color color;

public:
	SplashText(const std::string &, const Msp::GL::Font &);
	void set_color(const Msp::GL::Color &);
	void tick(const Msp::Time::TimeDelta &);
	void render() const;
};

#endif
