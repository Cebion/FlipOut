/* $Id: texture.h 65 2008-10-04 19:50:15Z tdb $

This file is part of libmspgl
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_GL_TEXTURE_H_
#define MSP_GL_TEXTURE_H_

#include <istream>
#include <msp/datafile/loader.h>
#include "gl.h"
#include "types.h"

namespace Msp {
namespace GL {

enum TextureFilter
{
	NEAREST                = GL_NEAREST,
	LINEAR                 = GL_LINEAR,
	NEAREST_MIPMAP_NEAREST = GL_NEAREST_MIPMAP_NEAREST,
	NEAREST_MIPMAP_LINEAR  = GL_NEAREST_MIPMAP_LINEAR,
	LINEAR_MIPMAP_NEAREST  = GL_LINEAR_MIPMAP_NEAREST,
	LINEAR_MIPMAP_LINEAR   = GL_LINEAR_MIPMAP_LINEAR
};

std::istream &operator>>(std::istream &, TextureFilter &);


/**
Base class for textures.  This class only defines operations common for all
texture types and is not instantiable.  For specifying images for textures, see
one of the dimensioned texture classes.
*/
class Texture
{
protected:
	class Loader: public DataFile::Loader
	{
	protected:
		Texture &tex;

	public:
		Loader(Texture &);
		void min_filter(TextureFilter);
		void mag_filter(TextureFilter);
		void generate_mipmap(bool);
	};

public:
	~Texture();

	void bind() const;
	void bind_to(unsigned) const;
	void parameter(GLenum, int);
	void parameter(GLenum, float);
	void set_min_filter(TextureFilter f) { parameter(GL_TEXTURE_MIN_FILTER, f); }
	void set_mag_filter(TextureFilter f) { parameter(GL_TEXTURE_MAG_FILTER, f); }
	GLenum get_target() const            { return target; }
	uint  get_id() const                 { return id; }

	static void unbind();
protected:
	uint   id;
	GLenum target;

	Texture();
	Texture(const Texture &);
	Texture &operator=(const Texture &);
	void maybe_bind() const;
};

} // namespace GL
} // namespace Msp

#endif
