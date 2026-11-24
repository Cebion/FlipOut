/* $Id: library.cpp 85 2009-02-21 00:45:50Z tdb $

This file is part of builder
Copyright © 2009  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include "library.h"

Library::Library(Builder &b, const Package *p, const Msp::FS::Path &a, const std::string &l):
	FileTarget(b, p, a),
	libname(l)
{ }
