/* $Id: font.h 70 2008-11-02 17:46:18Z tdb $

This file is part of libmspgl
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_GL_FONT_H_
#define MSP_GL_FONT_H_

#include <map>
#include <string>
#include <msp/datafile/collection.h>
#include <msp/strings/utf8.h>
#include "vertexarray.h"

namespace Msp {
namespace GL {

class PrimitiveBuilder;
class Texture2D;

class Font
{
public:
	class Loader: public DataFile::Loader
	{
	private:
		Font &font;
		DataFile::Collection *coll;

	public:
		typedef DataFile::Collection Collection;

		Loader(Font &);
		Loader(Font &, DataFile::Collection &);
		Font &get_object() { return font; }
		DataFile::Collection &get_collection();
	private:
		void init();
		void glyph(unsigned);
		void texture_inline();
	};

	Font();
	~Font();

	void  set_texture(const Texture2D &);
	const Texture2D &get_texture() const;
	void  add_glyph(unsigned, float, float, float, float, float, float, float, float, float);
	float get_default_size() const { return default_size; }
	float get_ascent() const { return ascent; }
	float get_descent() const { return descent; }
	float get_string_width(const std::string &, Codecs::Decoder &) const;
	void  draw_glyph(unsigned);
	void  draw_string(const std::string &, Codecs::Decoder &) const;
	void  draw_string(const std::string &, Codecs::Decoder &, PrimitiveBuilder &) const;

	template<class C>
	float get_string_width(const std::string &str) const
	{
		typename C::Decoder dec;
		return get_string_width(str, dec);
	}

	float get_string_width(const std::string &str) const
	{ return get_string_width<Codecs::Utf8>(str); }

	template<class C>
	void draw_string(const std::string &str) const
	{
		typename C::Decoder dec;
		draw_string(str, dec);
	}

	void draw_string(const std::string &str) const
	{ draw_string<Codecs::Utf8>(str); }

	template<class C>
	void draw_string(const std::string &str, PrimitiveBuilder &pb) const
	{
		typename C::Decoder dec;
		draw_string(str, dec, pb);
	}

	void draw_string(const std::string &str, PrimitiveBuilder &pb) const
	{ return draw_string<Codecs::Utf8>(str, pb); }

private:
	struct Glyph
	{
		class Loader: public Msp::DataFile::Loader
		{
		public:
			Loader(Glyph &);
			Glyph &get_object() { return glyph; }
		private:
			Glyph &glyph;

			void texcoords(float, float, float, float);
		};

		unsigned code;
		float x1,y1;
		float x2,y2;
		float w,h;
		float off_x, off_y;
		float advance;
	};
	typedef std::map<unsigned, Glyph> GlyphMap;

	const Texture2D *tex;
	bool     own_tex;
	float    default_size;
	float    ascent;
	float    descent;
	GlyphMap glyphs;

	void create_glyph_vertices(const Glyph &, VertexBuilder &, float) const;
	float get_glyph_advance(unsigned) const;
};

} // namespace GL
} // namespace Msp

#endif
