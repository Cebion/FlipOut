/* $Id: objectpass.cpp 50 2008-05-19 14:42:17Z tdb $

This file is part of libmspgl
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include "objectpass.h"
#include "program.h"
#include "programdata.h"

using namespace std;

namespace Msp {
namespace GL {

ObjectPass::ObjectPass():
	shprog(0),
	shdata(0),
	use_textures(true)
{ }

ObjectPass::~ObjectPass()
{
}


ObjectPass::Loader::Loader(ObjectPass &p, Collection &c):
	pass(p),
	coll(c)
{
	add("shader", &Loader::shader);
	add("use_textures", &ObjectPass::use_textures);
}

void ObjectPass::Loader::shader(const string &n)
{
	Program *shprog=coll.get<Program>(n);
	if(shprog)  // Allow for unsupported shaders
	{
		RefPtr<ProgramData> shdata=new ProgramData;
		load_sub(*shdata, *shprog);

		pass.shprog=shprog;
		if(pass.shdata)
			delete pass.shdata;
		pass.shdata=shdata.release();
	}
}

} // namespace GL
} // namespace Msp
