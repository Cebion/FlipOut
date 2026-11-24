/* $Id: graphic.cpp 31 2008-06-01 18:24:32Z tdb $

This file is part of libmspgltk
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include <msp/gl/immediate.h>
#include "graphic.h"
#include "resources.h"

using namespace std;

namespace Msp {
namespace GLtk {

Graphic::Graphic():
	texture(0),
	repeat(false)
{ }

void Graphic::render(unsigned wd, unsigned ht) const
{
	vector<float> x, y;
	create_coords(0.0f-shadow.left, wd+shadow.right, border.left, border.right, slice.w-border.left-border.right, x);
	create_coords(0.0f-shadow.bottom, ht+shadow.top, border.bottom, border.top, slice.h-border.bottom-border.top, y);

	vector<float> u, v;
	create_texcoords(slice.x, slice.x+slice.w, border.left, border.right, texture->get_width(), u);
	create_texcoords(slice.y, slice.y+slice.h, border.bottom, border.top, texture->get_height(), v);

	unsigned xmin=border.left ? 0 : 1;
	unsigned xmax=x.size()-(border.right ? 2 : 3);
	unsigned ymin=border.bottom ? 0 : 1;
	unsigned ymax=y.size()-(border.top ? 2 : 3);

	texture->bind();
	GL::Immediate imm((GL::COLOR4_UBYTE, GL::TEXCOORD2, GL::VERTEX2));
	imm.color(1.0f, 1.0f, 1.0f);
	imm.begin(GL::QUADS);
	for(unsigned i=ymin; i<=ymax; ++i)
	{
		unsigned i2=(i==0 ? 0 : i==y.size()-2 ? 2 : 1);
		for(unsigned j=xmin; j<=xmax; ++j)
		{
			unsigned j2=(j==0 ? 0 : j==x.size()-2 ? 2 : 1);
			imm.texcoord(u[j2], v[i2]);
			imm.vertex(x[j], y[i]);
			imm.texcoord(u[j2+1], v[i2]);
			imm.vertex(x[j+1], y[i]);
			imm.texcoord(u[j2+1], v[i2+1]);
			imm.vertex(x[j+1], y[i+1]);
			imm.texcoord(u[j2], v[i2+1]);
			imm.vertex(x[j], y[i+1]);
		}
	}
	imm.end();
}

void Graphic::create_coords(float low, float high, float brd1, float brd2, float block, vector<float> &coords) const
{
	coords.push_back(low);
	coords.push_back(low+brd1);
	if(repeat)
	{
		float space=high-low-brd1-brd2;
		unsigned div=max(static_cast<unsigned>(space/block), 1U);
		float delta=space/div;
		for(unsigned i=1; i<div; ++i)
			coords.push_back(low+brd1+delta*i);
	}
	coords.push_back(high-brd2);
	coords.push_back(high);
}

void Graphic::create_texcoords(float low, float high, float brd1, float brd2, float scale, vector<float> &coords) const
{
	coords.push_back(low/scale);
	coords.push_back((low+brd1)/scale);
	coords.push_back((high-brd2)/scale);
	coords.push_back(high/scale);
}


Graphic::Loader::Loader(Graphic &g, Resources &r):
	graph(g),
	res(r)
{
	add("texture", &Loader::texture);
	add("slice",   &Loader::slice);
	add("border",  &Loader::border);
	add("repeat",  &Graphic::repeat);
	add("shadow",  &Loader::shadow);
}

void Graphic::Loader::texture(const string &n)
{
	graph.texture=res.get<GL::Texture2D>(n);
	graph.slice=Geometry(0, 0, graph.texture->get_width(), graph.texture->get_height());
}

void Graphic::Loader::slice(unsigned x, unsigned y, unsigned w, unsigned h)
{
	graph.slice=Geometry(x, y, w, h);
}

void Graphic::Loader::border()
{
	load_sub(graph.border);
}

void Graphic::Loader::shadow()
{
	load_sub(graph.shadow);
}

} // namespace GLtk
} // namespace Msp
