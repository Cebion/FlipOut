/* $Id: table.h 30 2008-04-28 13:51:58Z tdb $

This file is part of libmspgltk
Copyright © 2008  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_GLTK_TABLE_H_
#define MSP_GLTK_TABLE_H_

#include <string>
#include <vector>
#include "widget.h"

namespace Msp {
namespace GLtk {

class Table: public Widget
{
public:
	class Loader: public Widget::Loader
	{
	public:
		Loader(Table &);
	private:
		void cell_text(unsigned, unsigned, const std::string &);
		void column_width(unsigned, unsigned);
		void columns(unsigned);
		void rows(unsigned);
	};

private:
	unsigned rows;
	unsigned columns;
	std::vector<std::string> data;
	std::vector<unsigned> col_w;

public:
	Table(const Resources &);

	void set_rows(unsigned);
	void set_columns(unsigned);
	unsigned get_rows() const { return rows; }
	unsigned get_columns() const { return columns; }

	void set_column_width(unsigned, unsigned);
	void set_cell_text(unsigned, unsigned, const std::string &);
private:
	virtual const char *get_class() const { return "table"; }

	virtual void render_special(const Part &) const;
	virtual void on_style_change();
};

} // namespace GLtk
} // namespace Msp

#endif
