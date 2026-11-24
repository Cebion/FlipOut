/* $Id: objectpass.h 49 2008-03-24 00:10:57Z tdb $

This file is part of libmspgl
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_GL_OBJECTPASS_H_
#define MSP_GL_OBJECTPASS_H_

#include <msp/datafile/collection.h>

namespace Msp {
namespace GL {

class Program;
class ProgramData;

struct ObjectPass
{
	class Loader: public DataFile::Loader
	{
	public:
		typedef DataFile::Collection Collection;

	private:
		ObjectPass &pass;
		Collection &coll;

	public:
		Loader(ObjectPass &, Collection &);
		ObjectPass &get_object() const { return pass; }
		Collection &get_collection() const { return coll; }
	private:
		void shader(const std::string &);
	};

	Program *shprog;
	ProgramData *shdata;
	bool use_textures;

	ObjectPass();
	~ObjectPass();
};

} // namespace GL
} // namespace Msp

#endif
