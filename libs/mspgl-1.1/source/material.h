/* $Id: material.h 62 2008-09-24 16:10:43Z tdb $

This file is part of libmspgl
Copyright © 2007-2008  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_GL_MATERIAL_H_
#define MSP_GL_MATERIAL_H_

#include <msp/datafile/loader.h>
#include "color.h"

namespace Msp {
namespace GL {

/**
Stores OpenGL material properties.  Since OpenGL does not support material
objects, application of material is done with several calls to glMaterial.
*/
class Material
{
public:
	class Loader: public DataFile::Loader
	{
	private:
		Material &mat;

	public:
		Loader(Material &);
		Material &get_object() const { return mat; }
	
	private:
		void ambient(float, float, float, float);
		void diffuse(float, float, float, float);
		void specular(float, float, float, float);
		void emission(float, float, float, float);
	};

private:
	Color ambient;
	Color diffuse;
	Color specular;
	Color emission;
	float shininess;

public:
	Material();
	void set_ambient(const Color &a);
	void set_diffuse(const Color &d);
	void set_specular(const Color &s);
	void set_emission(const Color &e);
	void set_shininess(float s);
	const Color &get_ambient() const { return ambient; }
	const Color &get_diffuse() const { return diffuse; }
	const Color &get_specular() const { return specular; }
	const Color &get_emission() const { return emission; }
	float get_shininess() const { return shininess; }
	void bind() const;

	static void unbind();
private:
	static const Material *current;
};

} // namespace GL
} // namespace Msp

#endif
