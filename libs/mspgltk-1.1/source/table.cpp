/* $Id: table.cpp 30 2008-04-28 13:51:58Z tdb $

This file is part of libmspgltk
Copyright © 2008  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include <msp/gl/matrix.h>
#include <msp/gl/transform.h>
#include "part.h"
#include "style.h"
#include "table.h"

using namespace std;

namespace Msp {
namespace GLtk {

Table::Table(const Resources &r):
	Widget(r),
	rows(1),
	columns(1),
	data(1),
	col_w(1)
{
	update_style();
}

void Table::set_rows(unsigned r)
{
	rows=r;
	data.resize(rows*columns);
}

void Table::set_columns(unsigned c)
{
	vector<string> new_data(rows*c);
	for(unsigned i=0; i<rows; ++i)
		for(unsigned j=0; j<min(c, columns); ++j)
			new_data[i*c+j].swap(data[i*columns+j]);
	data.swap(new_data);
	columns=c;

	col_w.resize(columns);
}

void Table::set_column_width(unsigned c, unsigned w)
{
	if(c>=columns)
		throw InvalidParameterValue("Column index out of bounds");
	col_w[c]=w;
}

void Table::set_cell_text(unsigned r, unsigned c, const string &t)
{
	if(r>=rows || c>=columns)
		throw InvalidParameterValue("Cell coordinates out of bounds");

	data[r*columns+c]=t;
}

void Table::render_special(const Part &part) const
{
	if(part.get_name()=="cells")
	{
		const GL::Font *const font=style->get_font();
		const float font_size=font->get_default_size();

		unsigned free_width=geom.w;
		for(unsigned i=0; i<columns; ++i)
			free_width-=col_w[i];

		Geometry cgeom;
		cgeom.h=geom.h/rows;
		cgeom.y=geom.h-cgeom.h;

		for(unsigned i=0; i<rows; ++i)
		{
			cgeom.x=0;
			for(unsigned j=0; j<columns; ++j)
			{
				if(col_w[j])
					cgeom.w=col_w[j];
				else
					cgeom.w=free_width/columns;

				const string &text=data[i*columns+j];
				Geometry rgeom;
				rgeom.x=cgeom.x;
				rgeom.y=cgeom.y;
				rgeom.w=static_cast<unsigned>(font->get_string_width(text)*font_size);
				rgeom.h=static_cast<unsigned>((font->get_ascent()-font->get_descent())*font_size);

				part.get_alignment().apply(rgeom, cgeom, part.get_margin());

				GL::push_matrix();
				GL::translate(rgeom.x, rgeom.y, 0);
				GL::scale_uniform(font_size);
				font->draw_string(text);
				GL::pop_matrix();

				cgeom.x+=cgeom.w;
			}

			cgeom.y-=cgeom.h;
		}
	}
}

void Table::on_style_change()
{
}


Table::Loader::Loader(Table &t):
	Widget::Loader(t)
{
	add("cell_text",    &Loader::cell_text);
	add("column_width", &Loader::column_width);
	add("columns",      &Loader::columns);
	add("rows",         &Loader::rows);
}

void Table::Loader::cell_text(unsigned r, unsigned c, const string &t)
{
	static_cast<Table &>(wdg).set_cell_text(r, c, t);
}

void Table::Loader::column_width(unsigned c, unsigned w)
{
	static_cast<Table &>(wdg).set_column_width(c, w);
}

void Table::Loader::columns(unsigned c)
{
	static_cast<Table &>(wdg).set_columns(c);
}

void Table::Loader::rows(unsigned r)
{
	static_cast<Table &>(wdg).set_rows(r);
}

} // namespace GLtk
} // namespace Msp
