/* $Id: main.cpp 63 2008-12-24 07:01:36Z tdb $

This file is part of libmspcore
Copyright © 2006 Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifdef WIN32
#include <windows.h>
#endif

#include "application.h"

#ifdef WIN32
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPSTR /*lpCmdLine*/, int /*nCmdShow*/)
{ 
	int argc	= 0;
	LPWSTR *argv=CommandLineToArgvW(GetCommandLineW(), &argc);
	return Msp::Application::run(argc, (char **)argv, hInstance);
}
#endif

int main(int argc, char **argv)
{ return Msp::Application::run(argc, argv); }
