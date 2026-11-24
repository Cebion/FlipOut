/* $Id: parser.cpp 53 2008-09-13 18:03:04Z tdb $

This file is part of libmspdatafile
Copyright © 2006  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/
#include <msp/strings/formatter.h>
#include "binaryparser.h"
#include "parser.h"
#include "statement.h"
#include "textparser.h"

using namespace std;

namespace Msp {
namespace DataFile {

Parser::Parser(IO::Base &i, const string &s):
	in(i),
	main_src(s),
	src(s),
	good(true),
	mode(new TextParser(in, src))
{ }

Parser::~Parser()
{
	delete mode;
}

Statement Parser::parse()
{
	if(!good)
		throw Exception("Parser is not good");

	try
	{
		while(1)
		{
			Statement st=mode->parse();
			if(st.keyword=="__bin")
			{
				delete mode;
				mode=new BinaryParser(in, src);
			}
			else if(st.keyword=="__text")
			{
				delete mode;
				mode=new TextParser(in, src);
			}
			else if(st.keyword=="__src")
			{
				string s=st.args[0].get<string>();
				if(s.empty())
					src=main_src;
				else
					src=format("%s[%s]", main_src, s);
			}
			else
				return st;
		}
	}
	catch(const Exception &e)
	{
		good=false;
		throw;
	}
}

} // namespace DataFile
} // namespace Msp
