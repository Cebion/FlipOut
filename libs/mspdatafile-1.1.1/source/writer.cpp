/* $Id: writer.cpp 41 2008-07-08 15:23:07Z tdb $

This file is part of libmspdatafile
Copyright © 2006  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include "binarywriter.h"
#include "statement.h"
#include "textwriter.h"
#include "writer.h"

using namespace std;

namespace Msp {
namespace DataFile {

Writer::Writer(IO::Base &o):
	out(o),
	mode(new TextWriter(out)),
	binary(false)
{ }

Writer::~Writer()
{
	delete mode;
}

void Writer::write(const Statement &st)
{
	mode->write(st);
}

void Writer::set_binary(bool b)
{
	if(b==binary)
		return;

	binary=b;

	Statement st;
	if(binary)
		st.keyword="__bin";
	else
		st.keyword="__text";

	mode->write(st);

	delete mode;
	if(binary)
		mode=new BinaryWriter(out);
	else
		mode=new TextWriter(out);
}

} // namespace DataFile
} // namespace Msp
