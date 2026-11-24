/* $Id: object.h 66 2008-10-04 20:26:55Z tdb $

This file is part of libmspgl
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_GL_OBJECT_H_
#define MSP_GL_OBJECT_H_

#include <vector>
#include <msp/datafile/collection.h>
#include "objectpass.h"
#include "renderable.h"

namespace Msp {
namespace GL {

class Material;
class Mesh;
class ObjectInstance;
class Technique;
class Texture;

/**
Stores data for a complete 3D object.  An Object must always have a Mesh, and
may also have a Technique to determine its appearance.  Textures and material
specified by the Technique may be overridden.  Simple textured objects are also
possible without a Technique.

It is possible to use a single Object for rendering multiple identical or
similar objects.  See class ObjectInstance.
*/
class Object: public Renderable
{
private:
	std::vector<const Mesh *> meshes;
	const Technique *technique;
	std::vector<const Texture *> textures;
	const Texture *main_texture;
	const Material *material;

public:
	class Loader: public DataFile::Loader
	{
	public:
		typedef DataFile::Collection Collection;

	protected:
		Object &obj;
		Collection &coll;
	
	public:
		Loader(Object &, Collection &);

		Object &get_object() const { return obj; }
		Collection &get_collection() const { return coll; }
	private:
		virtual void finish();
		void lod_mesh(unsigned, const std::string &);
		void material_inline();
		void mesh(const std::string &);
		void shader_texture(const std::string &);
		void technique(const std::string &);
		void texture(const std::string &);
	};

	Object();
	~Object();

	const Technique *get_technique() const { return technique; }

	virtual bool has_pass(const Tag &) const;

	/**
	Renders the object.  A tag can be provided to render a non-default pass.
	*/
	virtual void render(const Tag &tag=Tag()) const;

	/**
	Renders the object with an instance.  The instance's hook functions are
	called before and after drawing the mesh.  A tag may also be given to render
	a non-default pass.
	*/
	virtual void render(const ObjectInstance &, const Tag &tag=Tag()) const;

	/**
	Renders multiple instances of the object in one go.  This may improve
	performance, as the object-specific render setup only has to be done once.
	Each instance's hook functions will be called before and after drawing the
	mesh.
	*/
	template<typename Iter>
	void render(Iter begin, Iter end, const Tag &tag=Tag()) const
	{
		const ObjectPass *pass=get_pass(tag);
		setup_render(pass);
		for(Iter i=begin; i!=end; ++i)
			render_instance(**i, tag);
		finish_render(pass);
	}
private:
	const ObjectPass *get_pass(const Tag &) const;
	void setup_render(const ObjectPass *) const;
	void finish_render(const ObjectPass *) const;
	void render_instance(const ObjectInstance &, const Tag &) const;
};

} // namespace GL
} // namespace Msp

#endif
