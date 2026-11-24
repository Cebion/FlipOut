/* $Id: framebuffer.h 84 2009-09-21 13:50:58Z tdb $

This file is part of libmspgl
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_GL_FRAMEBUFFER_H_
#define MSP_GL_FRAMEBUFFER_H_

#include <vector>
#include "gl.h"
#include "types.h"

namespace Msp {
namespace GL {

class Renderbuffer;
class Texture;
class Texture2D;

enum FramebufferAttachment
{
	COLOR_ATTACHMENT0  = GL_COLOR_ATTACHMENT0_EXT,
	COLOR_ATTACHMENT1  = GL_COLOR_ATTACHMENT1_EXT,
	COLOR_ATTACHMENT2  = GL_COLOR_ATTACHMENT2_EXT,
	COLOR_ATTACHMENT3  = GL_COLOR_ATTACHMENT3_EXT,
	DEPTH_ATTACHMENT   = GL_DEPTH_ATTACHMENT_EXT,
	STENCIL_ATTACHMENT = GL_STENCIL_ATTACHMENT_EXT
};

enum FramebufferStatus
{
	FRAMEBUFFER_INCOMPLETE_ATTACHMENT         = GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT,
	FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT = GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT,
	FRAMEBUFFER_INCOMPLETE_DIMENSIONS         = GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT,
	FRAMEBUFFER_INCOMPLETE_FORMATS            = GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT,
	FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER        = GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT,
	FRAMEBUFFER_INCOMPLETE_READ_BUFFER        = GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT,
	FRAMEBUFFER_UNSUPPORTED                   = GL_FRAMEBUFFER_UNSUPPORTED_EXT,
	FRAMEBUFFER_COMPLETE                      = GL_FRAMEBUFFER_COMPLETE_EXT
};

enum BufferBits
{
	COLOR_BUFFER_BIT   = GL_COLOR_BUFFER_BIT,
	DEPTH_BUFFER_BIT   = GL_DEPTH_BUFFER_BIT,
	STENCIL_BUFFER_BIT = GL_STENCIL_BUFFER_BIT,
	ACCUM_BUFFER_BIT   = GL_ACCUM_BUFFER_BIT
};

enum RWBuffer
{
	NO_BUFFER      = GL_NONE,
	FRONT_LEFT     = GL_FRONT_LEFT,
	FRONT_RIGHT    = GL_FRONT_RIGHT,
	BACK_LEFT      = GL_BACK_LEFT,
	BACK_RIGHT     = GL_BACK_RIGHT,
	FRONT          = GL_FRONT,
	BACK           = GL_BACK,
	LEFT           = GL_LEFT,
	RIGHT          = GL_RIGHT,
	FRONT_AND_BACK = GL_FRONT_AND_BACK
};

/**
Framebuffer objects can be used to perform offscreen rendering.  The most
common application is rendering to a texture, which can then be used for
fullscreen shader effects.

A framebuffer consist of a number of logical buffers, such as color and depth
buffers.  Renderbuffers and Textures can be attached to the logical buffers.  At
least one image must be attached for the framebuffer to be usable.

Requires the GL_EXT_framebuffer_object extension.
*/
class Framebuffer
{
private:
	struct Attachment
	{
		FramebufferAttachment attachment;
		GLenum type;
		union
		{
			Renderbuffer *rbuf;
			Texture *tex;
		};

		Attachment(FramebufferAttachment);
		Attachment &operator=(Renderbuffer &);
		Attachment &operator=(Texture &);
	};

	uint id;
	std::vector<Attachment> attachments;
	unsigned width;
	unsigned height;

	static const Framebuffer *cur_fbo;
	static int sys_viewport[4];

public:
	Framebuffer();
	~Framebuffer();

	void bind() const;

	void attach(FramebufferAttachment attch, Renderbuffer &rbuf);
	void attach(FramebufferAttachment attch, Texture2D &tex, int level);
	void detach(FramebufferAttachment attch);

	/**
	Checks the completeness status of the framebuffer.  Returns
	FRAMEBUFFER_COMPLETE if the framebuffer is complate and can be rendered to,
	or one of the error status codes otherwise.
	*/
	FramebufferStatus check_status() const;

	static const Framebuffer *current();
	static void unbind();
private:
	void maybe_bind() const;
	Attachment &get_or_create_attachment(FramebufferAttachment);
	void check_size();
};

inline BufferBits operator|(BufferBits a, BufferBits b)
{ return static_cast<BufferBits>(static_cast<int>(a)|static_cast<int>(b)); }

void viewport(int, int, unsigned, unsigned);
void clear(BufferBits);
void draw_buffer(RWBuffer);
void read_buffer(RWBuffer);

} // namespace GL
} // namespace Msp

#endif
