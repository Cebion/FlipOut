/* $Id: feature.h 84 2009-02-19 16:43:37Z tdb $

This file is part of builder
Copyright © 2007, 2009  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef FEATURE_H_
#define FEATURE_H_

struct Feature
{
	std::string name;
	std::string descr;

	Feature(const std::string &n, const std::string &d): name(n), descr(d) { }
};

typedef std::list<Feature> FeatureList;

#endif
