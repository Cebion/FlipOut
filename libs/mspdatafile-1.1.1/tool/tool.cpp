/* $Id: tool.cpp 52 2008-09-12 18:58:06Z tdb $

This file is part of libmspdatafile
Copyright © 2008  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include <msp/core/getopt.h>
#include <msp/io/buffered.h>
#include <msp/io/console.h>
#include <msp/io/file.h>
#include <msp/datafile/parser.h>
#include <msp/datafile/statement.h>
#include <msp/datafile/writer.h>
#include "compiler.h"
#include "tool.h"

using namespace std;
using namespace Msp;

DataTool::DataTool(int argc, char **argv):
	in_fn("-"),
	out_fn("-"),
	binary(false),
	compile(false)
{
	GetOpt getopt;
	getopt.add_option('b', "binary", binary, GetOpt::NO_ARG);
	getopt.add_option('c', "compile", compile, GetOpt::NO_ARG);
	getopt.add_option('o', "output", out_fn, GetOpt::REQUIRED_ARG);
	getopt(argc, argv);

	const vector<string> &args=getopt.get_args();
	if(!args.empty())
		in_fn=args[0];
}

int DataTool::main()
{
	IO::Base *in;
	if(in_fn=="-")
		in=&IO::cin;
	else
		in=new IO::File(in_fn);

	IO::Base *out;
	if(out_fn=="-")
		out=&IO::cout;
	else
		out=new IO::File(out_fn, IO::M_WRITE);

	{
		IO::Buffered in_buf(*in);
		DataFile::Parser parser(in_buf, in_fn);
		IO::Buffered out_buf(*out);
		DataFile::Writer writer(out_buf);
		if(binary)
			writer.set_binary(true);

		if(compile)
		{
			Compiler compiler(writer);
			compiler.load(parser);
		}
		else
		{
			while(parser)
			{
				DataFile::Statement st=parser.parse();
				if(st.valid)
				{
					writer.write(st);
					out_buf.flush();
				}
			}
		}
	}

	if(in!=&IO::cin)
		delete in;
	if(out!=&IO::cout)
		delete out;

	return 0;
}

Application::RegApp<DataTool> DataTool::reg;
