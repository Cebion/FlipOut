/* $Id: pixelformat.cpp 55 2008-08-17 22:16:15Z tdb $

This file is part of libmspgl
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include "except.h"
#include "pixelformat.h"

using namespace std;

namespace Msp {
namespace GL {

istream &operator>>(istream &in, PixelFormat &fmt)
{
	string word;

	in>>word;
	if(word=="COLOR_INDEX")
		fmt=COLOR_INDEX;
	else if(word=="STENCIL_INDEX")
		fmt=STENCIL_INDEX;
	else if(word=="DEPTH_COMPONENT")
		fmt=DEPTH_COMPONENT;
	else if(word=="RED")
		fmt=RED;
	else if(word=="GREEN")
		fmt=GREEN;
	else if(word=="BLUE")
		fmt=BLUE;
	else if(word=="ALPHA")
		fmt=ALPHA;
	else if(word=="RGB")
		fmt=RGB;
	else if(word=="RGBA")
		fmt=RGBA;
	else if(word=="BGR")
		fmt=BGR;
	else if(word=="BGRA")
		fmt=BGRA;
	else if(word=="LUMINANCE")
		fmt=LUMINANCE;
	else if(word=="LUMINANCE_ALPHA")
		fmt=LUMINANCE_ALPHA;
	else
		in.setstate(ios_base::failbit);

	return in;
}

PixelFormat pixelformat_from_graphics(Graphics::PixelFormat pf)
{
	switch(pf)
	{
	case Graphics::COLOR_INDEX: return COLOR_INDEX;
	case Graphics::LUMINANCE: return LUMINANCE;
	case Graphics::LUMINANCE_ALPHA: return LUMINANCE_ALPHA;
	case Graphics::RGB: return RGB;
	case Graphics::RGBA: return RGBA;
	case Graphics::BGR: return BGR;
	case Graphics::BGRA: return BGRA;
	default: throw InvalidParameterValue("Unknown Graphics::PixelFormat");
	}
}

} // namespace GL
} // namespace Msp
