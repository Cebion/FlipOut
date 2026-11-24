/* $Id: textwriter.cpp 37 2008-02-04 15:00:02Z tdb $

This file is part of libmspdatafile
Copyright © 2006  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include <msp/io/print.h>
#include <msp/strings/utils.h>
#include "statement.h"
#include "textwriter.h"

using namespace std;

namespace Msp {
namespace DataFile {

TextWriter::TextWriter(IO::Base &o):
	WriterMode(o)
{ }

void TextWriter::write(const Statement &st)
{
	write_(st, 0);
}

void TextWriter::write_(const Statement &st, unsigned level)
{
	string indent(level, '\t');

	IO::print(out, "%s%s", indent, st.keyword);
	for(ValueArray::const_iterator i=st.args.begin(); i!=st.args.end(); ++i)
	{
		out.put(' ');
		if(i->get_type()==STRING)
			IO::print(out, "\"%s\"", c_escape(i->get_raw(), false));
		else if(i->get_type()==BOOLEAN)
			out.write(i->get<bool>() ? "true" : "false");
		else
			out.write(i->get_raw());
	}
	if(!st.sub.empty())
	{
		IO::print(out, "\n%s{\n", indent);
		for(list<Statement>::const_iterator i=st.sub.begin(); i!=st.sub.end(); ++i)
			write_(*i, level+1);
		IO::print(out, "%s}", indent);
	}
	out.write(";\n", 2);
}

} // namespace DataFile
} // namespace Msp
