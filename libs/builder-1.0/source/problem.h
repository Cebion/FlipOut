/* $Id: problem.h 84 2009-02-19 16:43:37Z tdb $

This file is part of builder
Copyright © 2007, 2009  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef PROBLEM_H_
#define PROBLEM_H_

#include <list>
#include <string>

class Package;

struct Problem
{
	std::string package;
	std::string descr;

	Problem(const std::string &p, const std::string &d): package(p), descr(d) { }
};

typedef std::list<Problem> ProblemList;

#endif
