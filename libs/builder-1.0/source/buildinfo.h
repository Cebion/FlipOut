/* $Id: buildinfo.h 84 2009-02-19 16:43:37Z tdb $

This file is part of builder
Copyright © 2006-2009  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef BUILDINFO_H_
#define BUILDINFO_H_

#include <list>
#include <string>
#include <msp/datafile/loader.h>
#include "misc.h"

/**
Stores information about compiler command line parameters in a more abstract
form.  Allows combining with other BuildInfos to support package dependencies.
*/
class BuildInfo
{
public:
	class Loader: public Msp::DataFile::Loader
	{
	private:
		BuildInfo &binfo;
		
	public:
		Loader(BuildInfo &);
	private:
		void cflag(const std::string &);
		void incpath(const std::string &);
		void define(const std::string &);
		void ldflag(const std::string &);
		void libpath(const std::string &);
		void library(const std::string &);
		void warning(const std::string &);
	};
	
	StringList cflags;
	StringList defines;
	StringList incpath;
	StringList ldflags;
	StringList libpath;
	StringList libs;
	StringList warnings;

	/** Adds another BuildInfo to the end of this one. */
	void add(const BuildInfo &);

	/** Makes sure there are no duplicate entries in the lists.  For warnings,
	contradicting flags are eliminated and the last one stays in effect. */
	void unique();
};

#endif
