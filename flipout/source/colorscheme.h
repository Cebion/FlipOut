/* $Id$

This file is part of flipout
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef COLORSCHEME_H_
#define COLORSCHEME_H_

#include "brickcolor.h"

class ColorScheme
{
public:
	class Loader: public Msp::DataFile::Loader
	{
	private:
		ColorScheme &scheme;
	
	public:
		Loader(ColorScheme &);
	private:
		void color(const std::string &);
	};

	std::map<std::string, BrickColor> colors;

	void add(const std::string &, const BrickColor &);
	void add(const ColorScheme &);
	const BrickColor &get(const std::string & =std::string()) const;
};

#endif
