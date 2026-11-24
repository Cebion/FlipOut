/* $Id: technique.h 83 2009-08-03 12:25:44Z tdb $

This file is part of libmspgl
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef TECHNIQUE_H_
#define TECHNIQUE_H_

#include "objectpass.h"

namespace Msp {
namespace GL {

class Material;
class Tag;
class Texture;

/**
Stores a complete multipass rendering technique for an Object.  This includes
shaders, textures and a material.  A Technique can also specify empty texture
slots which Objects must override.
*/
class Technique
{
public:
	class Loader: public Msp::DataFile::Loader
	{
	public:
		typedef DataFile::Collection Collection;

	protected:
		Technique &tech;
		Collection &coll;
	
	public:
		Loader(Technique &, Collection &);

		Technique &get_object() const { return tech; }
		Collection &get_collection() const { return coll; }
	private:
		virtual void finish();
		void material_inline();
		void pass(const std::string &);
		void shader(const std::string &);
		void shader_texture(const std::string &);
		void texture(const std::string &);
		void texture_slot(const std::string &);
	};

private:
	std::vector<std::string> tex_names;
	std::vector<const Texture *> textures;
	const Texture *main_texture;
	std::map<unsigned, ObjectPass> passes;
	ObjectPass *normal_pass;
	const Material *material;

public:
	Technique();
	~Technique();

	bool has_pass(const GL::Tag &) const;
	const ObjectPass &get_pass(const GL::Tag &) const;
	unsigned get_n_textures() const { return tex_names.size(); }
	unsigned get_texture_index(const std::string &) const;
	const Texture *get_texture(unsigned) const;
	const Texture *get_main_texture() const { return main_texture; }
	const Material *get_material() const { return material; }
};

} // namespace GL
} // namespace Msp

#endif
