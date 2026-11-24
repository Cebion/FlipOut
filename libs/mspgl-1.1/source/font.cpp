/* $Id: font.cpp 70 2008-11-02 17:46:18Z tdb $

This file is part of libmspgl
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include "gl.h"
#include "font.h"
#include "immediate.h"
#include "primitivetype.h"
#include "texture2d.h"

using namespace std;

namespace Msp {
namespace GL {

Font::Font():
	tex(0),
	own_tex(false),
	default_size(1),
	ascent(1),
	descent(0)
{ }

Font::~Font()
{
	if(own_tex)
		delete tex;
}

void Font::set_texture(const Texture2D &t)
{
	if(own_tex)
		delete tex;

	tex=&t;
	own_tex=false;
}

const Texture2D &Font::get_texture() const
{
	if(!tex)
		throw InvalidState("No texture");
	return *tex;
}

void Font::add_glyph(unsigned code, float x1, float y1, float x2, float y2, float w, float h, float ox, float oy, float adv)
{
	Glyph glyph;
	glyph.code=code;
	glyph.x1=x1;
	glyph.y1=y1;
	glyph.x2=x2;
	glyph.y2=y2;
	glyph.w=w;
	glyph.h=h;
	glyph.off_x=ox;
	glyph.off_y=oy;
	glyph.advance=adv;
	glyphs.insert(GlyphMap::value_type(code, glyph));
}

float Font::get_string_width(const string &str, Codecs::Decoder &dec) const
{
	float x=0;

	for(string::const_iterator i=str.begin(); i!=str.end();)
		x+=get_glyph_advance(dec.decode_char(str, i));

	return x;
}

void Font::draw_string(const string &str, Codecs::Decoder &dec) const
{
	Immediate imm((TEXCOORD2, VERTEX2));
	draw_string(str, dec, imm);
}

void Font::draw_string(const string &str, Codecs::Decoder &dec, PrimitiveBuilder &pbuilder) const
{
	if(!tex)
		throw InvalidState("No texture");

	tex->bind();

	float x=0;
	unsigned count=0;

	pbuilder.begin(QUADS);
	for(string::const_iterator i=str.begin(); i!=str.end();)
	{
		GlyphMap::const_iterator j=glyphs.find(dec.decode_char(str, i));
		if(j==glyphs.end())
			continue;

		create_glyph_vertices(j->second, pbuilder, x);
		x+=j->second.advance;
		count+=4;
	}
	pbuilder.end();
}

void Font::create_glyph_vertices(const Glyph &glyph, VertexBuilder &vbuilder, float x) const
{
	vbuilder.texcoord(glyph.x1, glyph.y1);
	vbuilder.vertex(x+glyph.off_x, glyph.off_y);
	vbuilder.texcoord(glyph.x2, glyph.y1);
	vbuilder.vertex(x+glyph.off_x+glyph.w, glyph.off_y);
	vbuilder.texcoord(glyph.x2, glyph.y2);
	vbuilder.vertex(x+glyph.off_x+glyph.w, glyph.off_y+glyph.h);
	vbuilder.texcoord(glyph.x1, glyph.y2);
	vbuilder.vertex(x+glyph.off_x, glyph.off_y+glyph.h);
}

float Font::get_glyph_advance(unsigned code) const
{
	GlyphMap::const_iterator i=glyphs.find(code);
	if(i==glyphs.end())
		return 0;

	return i->second.advance;
}


Font::Loader::Loader(Font &f):
	font(f),
	coll(0)
{
	init();
}

Font::Loader::Loader(Font &f, Collection &c):
	font(f),
	coll(&c)
{
	init();
}

DataFile::Collection &Font::Loader::get_collection()
{
	if(!coll)
		throw InvalidState("No collection");
	return *coll;
}

void Font::Loader::init()
{
	add("default_size", &Font::default_size);
	add("ascent",  &Font::ascent);
	add("descent", &Font::descent);
	add("texture", &Font::tex);
	add("texture_inline", &Loader::texture_inline);
	add("glyph",   &Loader::glyph);
}

void Font::Loader::glyph(unsigned c)
{
	Glyph gl;
	gl.code=c;
	load_sub(gl);
	font.glyphs.insert(GlyphMap::value_type(c, gl));
}

void Font::Loader::texture_inline()
{
	RefPtr<Texture2D> tex=new Texture2D;
	load_sub(*tex);
	font.tex=tex.release();
	font.own_tex=true;
}


Font::Glyph::Loader::Loader(Glyph &g):
	glyph(g)
{
	add("texcoords", &Loader::texcoords);
	add("size",      &Glyph::w,     &Glyph::h);
	add("offset",    &Glyph::off_x, &Glyph::off_y);
	add("advance",   &Glyph::advance);
}

void Font::Glyph::Loader::texcoords(float x1, float y1, float x2, float y2)
{
	glyph.x1=x1;
	glyph.y1=y1;
	glyph.x2=x2;
	glyph.y2=y2;
}

} // namespace GL
} // namespace Msp
