/* $Id: object.cpp 83 2009-08-03 12:25:44Z tdb $

This file is part of libmspgl
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include <msp/strings/formatter.h>
#include "except.h"
#include "material.h"
#include "mesh.h"
#include "object.h"
#include "objectinstance.h"
#include "objectpass.h"
#include "program.h"
#include "programdata.h"
#include "technique.h"
#include "texture.h"
#include "texunit.h"

using namespace std;

namespace Msp {
namespace GL {

Object::Object():
	meshes(1, static_cast<Mesh *>(0)),
	technique(0),
	main_texture(0),
	material(0)
{ }

Object::~Object()
{
}

bool Object::has_pass(const Tag &tag) const
{
	if(technique)
		return technique->has_pass(tag);
	else
		return tag.id==0;
}

void Object::render(const Tag &tag) const
{
	const ObjectPass *pass=get_pass(tag);
	setup_render(pass);
	meshes[0]->draw();
	finish_render(pass);
}

void Object::render(const ObjectInstance &inst, const Tag &tag) const
{
	const ObjectPass *pass=get_pass(tag);
	setup_render(pass);
	render_instance(inst, tag);
	meshes[0]->draw();
	finish_render(pass);
}

const ObjectPass *Object::get_pass(const Tag &tag) const
{
	if(technique)
		return &technique->get_pass(tag);
	else if(tag.id==0)
		return 0;
	throw KeyError("Unknown pass");
}

void Object::setup_render(const ObjectPass *pass) const
{
	if(!meshes[0])
		throw InvalidState("Trying to render Object without mesh");

	if(pass && pass->shprog)
	{
		pass->shprog->bind();
		pass->shdata->apply();
		for(unsigned i=0; i<textures.size(); ++i)
		{
			TexUnit::activate(i);
			textures[i]->bind();
		}
	}
	else if(main_texture && (!pass || pass->use_textures))
		main_texture->bind();

	if(material)
		material->bind();
}

void Object::finish_render(const ObjectPass *pass) const
{
	if(pass && pass->shprog)
	{
		Program::unbind();
		for(unsigned i=textures.size(); i--;)
		{
			TexUnit::activate(i);
			Texture::unbind();
		}
	}
	else if(main_texture)
		Texture::unbind();

	if(material)
		Material::unbind();
}

void Object::render_instance(const ObjectInstance &inst, const Tag &tag) const
{
	inst.setup_render(tag);
	unsigned lod=min(inst.get_level_of_detail(), meshes.size()-1);
	meshes[lod]->draw();
	inst.finish_render(tag);
}


Object::Loader::Loader(Object &o, Collection &c):
	obj(o),
	coll(c)
{
	add("lod_mesh", &Loader::lod_mesh);
	add("material", &Object::material);
	add("material_inline", &Loader::material_inline);
	add("mesh",     &Loader::mesh);
	add("shader_texture", &Loader::shader_texture);
	add("technique", &Loader::technique);
	add("texture",  &Loader::texture);
}

void Object::Loader::finish()
{
	if(obj.technique && !obj.main_texture)
		obj.main_texture=obj.technique->get_main_texture();
	for(unsigned i=0; i<obj.textures.size(); ++i)
	{
		if(!obj.textures[i])
		{
			obj.textures[i]=obj.technique->get_texture(i);
			if(!obj.textures[i])
				throw Exception("Object does not specify all textures required by Technique");
		}
	}
}

void Object::Loader::lod_mesh(unsigned l, const string &n)
{
	obj.meshes.resize(l+1, 0);
	obj.meshes[l]=coll.get<Mesh>(n);
}

void Object::Loader::material_inline()
{
	RefPtr<Material> mat=new Material;
	load_sub(*mat);
	coll.add(format("_%p", mat.get()), mat.get());
	obj.material=mat.release();
}

void Object::Loader::mesh(const string &n)
{
	obj.meshes[0]=coll.get<Mesh>(n);
}

void Object::Loader::shader_texture(const string &n)
{
	if(!obj.technique)
		throw InvalidState("Can't specify shader textures without a Technique");

	unsigned eqsign=n.find('=');
	if(eqsign==string::npos)
		throw InvalidParameterValue("Must specify texture slot name");

	obj.textures[obj.technique->get_texture_index(n.substr(0, eqsign))]=coll.get<Texture>(n.substr(eqsign+1));
}

void Object::Loader::technique(const string &n)
{
	obj.technique=coll.get<Technique>(n);
	obj.textures.resize(obj.technique->get_n_textures());
	obj.material=obj.technique->get_material();
}

void Object::Loader::texture(const string &n)
{
	if(obj.main_texture)
		throw Exception("Only one main texture may be specified");

	Texture *tex=coll.get<Texture>(n);
	if(obj.technique)
		obj.textures[obj.technique->get_texture_index("texture")]=tex;
	obj.main_texture=tex;
}

} // namespace GL
} // namespace Msp
