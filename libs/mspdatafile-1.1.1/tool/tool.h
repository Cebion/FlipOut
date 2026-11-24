/* $Id: tool.h 52 2008-09-12 18:58:06Z tdb $

This file is part of libmspdatafile
Copyright © 2008  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef TOOL_H_
#define TOOL_H_

#include <string>
#include <msp/core/application.h>

class DataTool: public Msp::Application
{
private:
	std::string in_fn;
	std::string out_fn;
	bool binary;
	bool compile;

public:
	DataTool(int argc, char **argv);
	int main();

	static Application::RegApp<DataTool> reg;
};

#endif
