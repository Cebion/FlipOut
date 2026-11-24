/* $Id: install.h 96 2009-09-11 22:48:06Z tdb $

This file is part of builder
Copyright © 2006-2009  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef INSTALL_H_
#define INSTALL_H_

#include "sourcepackage.h"
#include "filetarget.h"

/**
Represents the installation of a file.
*/
class Install: public FileTarget
{
private:
	FileTarget &source;

public:
	Install(Builder &, const SourcePackage &, FileTarget &, const std::string & =std::string());
	virtual const char *get_type() const { return "Install"; }
	FileTarget &get_source() const { return source; }
private:
	virtual void check_rebuild();
	virtual Action *create_action();

	static Msp::FS::Path generate_target_path(const FileTarget &i, const std::string &);
};

#endif
