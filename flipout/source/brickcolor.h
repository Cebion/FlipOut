/* $Id$

This file is part of flipout
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef BRICKCOLOR_H_
#define BRICKCOLOR_H_

#include <msp/datafile/loader.h>
#include <msp/gl/color.h>

struct BrickColor
{
	class Loader: public Msp::DataFile::Loader
	{
	private:
		BrickColor &bcolor;

	public:
		Loader(BrickColor &);
	private:
		void color(float, float, float);
		void variation(float, float, float);
	};

	Msp::GL::Color color;
	Msp::GL::Color variation;

	BrickColor();
	Msp::GL::Color random() const;
};

#endif
