/* $Id: compiler.h 53 2008-09-13 18:03:04Z tdb $

This file is part of libmspdatafile
Copyright © 2008  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef TOOL_COMPILER_H_
#define TOOL_COMPILER_H_

#include <msp/datafile/loader.h>
#include <msp/datafile/statement.h>
#include <msp/datafile/writer.h>
#include <msp/fs/path.h>

class Compiler: public Msp::DataFile::Loader
{
	friend class File;
	friend class ForEach;

private:
	Msp::DataFile::Writer &writer;
	bool reset_src;

public:
	Compiler(Msp::DataFile::Writer &);
private:
	void file(const std::string &);
	void for_each(const std::vector<std::string> &);
	void write(const Msp::DataFile::Statement &);

	bool process_statement(const Msp::FS::Path &, Msp::DataFile::Statement &);
	void process_file(const Msp::FS::Path &, const std::list<Msp::DataFile::Statement> &);
	void process_file(const Msp::FS::Path &);
};


class File: public Msp::DataFile::Loader
{
private:
	Compiler &compiler;
	Msp::FS::Path filename;
	std::list<Msp::DataFile::Statement> write_st;

public:
	File(Compiler &, const Msp::FS::Path &);
private:
	virtual void finish();

	void write(const Msp::DataFile::Statement &);
};


class ForEach: public Msp::DataFile::Loader
{
private:
	Compiler &compiler;
	Msp::FS::Path base;
	std::list<std::string> patterns;
	std::list<std::string> excludes;
	std::list<Msp::DataFile::Statement> write_st;

public:
	ForEach(Compiler &, const Msp::FS::Path &, const std::list<std::string> &);
private:
	virtual void finish();

	void exclude(const std::string &);
	void pattern(const std::string &);
	void write(const Msp::DataFile::Statement &);
};

#endif
