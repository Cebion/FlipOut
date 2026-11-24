#include <msp/gl/immediate.h>
#include <msp/gl/matrix.h>
#include "style.h"
#include "text.h"

using namespace std;

namespace Msp {
namespace GLtk {

Text::Text(const Style *const &s):
	style(s)
{ }

Text::Text(const Style *const &s, const string &t):
	style(s)
{
	set(t);
}

unsigned Text::get_width() const
{
	unsigned width=0;
	for(vector<Line>::const_iterator i=lines.begin(); i!=lines.end(); ++i)
		width=max(width, i->width);
	return width;
}

unsigned Text::get_height() const
{
	const GL::Font *font=style->get_font();
	float font_size=font->get_default_size();
	unsigned line_height=static_cast<unsigned>((font->get_ascent()-font->get_descent())*font_size);
	unsigned line_spacing=line_height*6/5;
	return line_height+(lines.size()-1)*line_spacing;
}

void Text::set(const string &t)
{
	text=t;
	lines.clear();
	float font_size=style->get_font()->get_default_size();
	unsigned start=0;
	while(1)
	{
		unsigned newline=text.find('\n', start);

		Line line;
		line.start=start;
		line.length=(newline==string::npos ? text.size() : newline)-start;
		line.width=static_cast<unsigned>(style->get_font()->get_string_width(text.substr(line.start, line.length))*font_size);
		lines.push_back(line);

		if(newline==string::npos)
			break;
		start=newline+1;
	}
}

void Text::erase(unsigned pos, unsigned len)
{
	text.erase(pos, len);
}

void Text::insert(unsigned pos, const string &s)
{
	text.insert(pos, s);
}

void Text::render(const Part &part, const Geometry &geom) const
{
	if(lines.empty())
		return;

	const GL::Font *font=style->get_font();
	float font_size=font->get_default_size();
	unsigned line_height=static_cast<unsigned>((font->get_ascent()-font->get_descent())*font_size);
	unsigned line_spacing=font_size*6/5;
	unsigned height=line_height+(lines.size()-1)*line_spacing;
	int y_offset=static_cast<int>(-font->get_descent()*font_size);

	const GL::Color &color=style->get_font_color();
	GL::Immediate imm((GL::COLOR4_UBYTE, GL::TEXCOORD2, GL::VERTEX2));
	imm.color(color.r, color.g, color.b);
	for(unsigned i=0; i<lines.size(); ++i)
	{
		const Line &line=lines[i];

		Geometry rgeom;
		rgeom.w=line.width;
		rgeom.h=height;
		rgeom.y=(lines.size()-1-i)*line_spacing+y_offset;
		part.get_alignment().apply(rgeom, geom, part.get_margin());

		GL::push_matrix();
		GL::translate(rgeom.x, rgeom.y, 0);
		GL::scale_uniform(font_size);

		font->draw_string(text.substr(line.start, line.length), imm);

		GL::pop_matrix();
	}
}

Text &Text::operator=(const string &t)
{
	set(t);
	return *this;
}

} // namespace GLtk
} // namespace Msp
