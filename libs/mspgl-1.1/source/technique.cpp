/* $Id: technique.cpp 66 2008-10-04 20:26:55Z tdb $

This file is part of libmspgl
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include <msp/strings/formatter.h>
#include "material.h"
#include "program.h"
#include "programdata.h"
#include "tag.h"
#include "technique.h"
#include "texture.h"

using namespace std;

namespace Msp {
namespace GL {

Technique::Technique():
	main_texture(0),
	normal_pass(&passes[0]),
	material(0)
{ }

Technique::~Technique()
{
	for(map<unsigned, ObjectPass>::iterator i=passes.begin(); i!=passes.end(); ++i)
		delete i->second.shdata;
}

bool Technique::has_pass(const GL::Tag &tag) const
{
	return passes.count(tag.id);
}

const ObjectPass &Technique::get_pass(const GL::Tag &tag) const
{
	map<unsigned, ObjectPass>::const_iterator i=passes.find(tag.id);
	if(i==passes.end())
		throw KeyError("Unknown pass");
	return i->second;
}

unsigned Technique::get_texture_index(const std::string &n) const
{
	for(unsigned i=0; i<tex_names.size(); ++i)
		if(tex_names[i]==n)
			return i;

	throw KeyError("Unknown texture slot", n);
}

const Texture *Technique::get_texture(unsigned i) const
{
	if(i>=textures.size())
		throw KeyError("Texture index out of range");

	return textures[i];
}


Technique::Loader::Loader(Technique &t, Collection &c):
	tech(t),
	coll(c)
{
	add("material",        &Technique::material);
	add("material_inline", &Loader::material_inline);
	add("pass",            &Loader::pass);
	add("shader",          &Loader::shader);
	add("shader_texture",  &Loader::shader_texture);
	add("texture",         &Loader::texture);
	add("texture_slot",    &Loader::texture_slot);
}

void Technique::Loader::finish()
{
	for(map<unsigned, ObjectPass>::iterator i=tech.passes.begin(); i!=tech.passes.end(); ++i)
		if(i->second.shdata)
		{
			for(unsigned j=0; j<tech.textures.size(); ++j)
				i->second.shdata->uniform(i->second.shprog->get_uniform_location(tech.tex_names[j]), static_cast<int>(j));
		}
}

void Technique::Loader::material_inline()
{
	RefPtr<Material> mat=new Material;
	load_sub(*mat);
	coll.add(format("_%p", mat.get()), mat.get());
	tech.material=mat.release();
}

void Technique::Loader::pass(const string &n)
{
	unsigned id=Tag(n).id;
	if(tech.passes.count(id))
		throw KeyError("Duplicate pass name", n);
	ObjectPass p;
	load_sub(p, coll);
	tech.passes[id]=p;
}

void Technique::Loader::shader(const string &n)
{
	Program *shprog=coll.get<Program>(n);
	if(shprog)  // Allow for unsupported shaders
	{
		RefPtr<ProgramData> shdata=new ProgramData;
		load_sub(*shdata, *shprog);

		tech.normal_pass->shprog=shprog;
		if(tech.normal_pass->shdata)
			delete tech.normal_pass->shdata;
		tech.normal_pass->shdata=shdata.release();
	}
}

void Technique::Loader::shader_texture(const string &n)
{
	unsigned eqsign=n.find('=');
	if(eqsign!=string::npos)
	{
		tech.textures.push_back(coll.get<Texture>(n.substr(eqsign+1)));
		tech.tex_names.push_back(n.substr(0, eqsign));
	}
	else
	{
		tech.textures.push_back(coll.get<Texture>(n));
		tech.tex_names.push_back(n);
	}
}

void Technique::Loader::texture(const string &n)
{
	if(tech.main_texture)
		throw Exception("Only one main texture may be specified");

	tech.main_texture=coll.get<Texture>(n);
	tech.textures.push_back(tech.main_texture);
	tech.tex_names.push_back("texture");
}

void Technique::Loader::texture_slot(const string &n)
{
	tech.tex_names.push_back(n);
	tech.textures.push_back(0);
}

} // namespace GL
} // namespace Msp
