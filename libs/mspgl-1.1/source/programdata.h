/* $Id: programdata.h 49 2008-03-24 00:10:57Z tdb $

This file is part of libmspgl
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_GL_PROGRAMDATA_H_
#define MSP_GL_PROGRAMDATA_H_

#include <map>
#include <msp/datafile/loader.h>

namespace Msp {
namespace GL {

class Program;
class Uniform;

/**
Stores uniform variables for a shader program.
*/
class ProgramData
{
public:
	class Loader: public DataFile::Loader
	{
	private:
		ProgramData &pdata;
		Program &prog;

	public:
		Loader(ProgramData &, Program &);
	private:
		void uniform1i(const std::string &, int);
		void uniform1f(const std::string &, float);
		void uniform2f(const std::string &, float, float);
		void uniform3f(const std::string &, float, float, float);
		void uniform4f(const std::string &, float, float, float, float);
	};

private:
	std::map<int, Uniform *> data;

	ProgramData(const ProgramData &);
	ProgramData &operator=(const ProgramData &);
public:
	ProgramData();
	~ProgramData();

	void uniform(int, Uniform *);
	void uniform(int, int);
	void uniform(int, float);
	void uniform(int, float, float);
	void uniform2(int, const float *);
	void uniform(int, float, float, float);
	void uniform3(int, const float *);
	void uniform(int, float, float, float, float);
	void uniform4(int, const float *);
	void uniform_matrix4(int, const float *);
	void apply() const;
};

} // namespace GL
} // namespace Msp

#endif
