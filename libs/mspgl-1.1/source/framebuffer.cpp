/* $Id: framebuffer.cpp 84 2009-09-21 13:50:58Z tdb $

This file is part of libmspgl
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include "extension.h"
#include "ext_framebuffer_object.h"
#include "framebuffer.h"
#include "misc.h"
#include "renderbuffer.h"
#include "texture2d.h"

using namespace std;

namespace Msp {
namespace GL {

Framebuffer::Framebuffer():
	width(0),
	height(0)
{
	static RequireExtension _ext("GL_EXT_framebuffer_object");

	glGenFramebuffersEXT(1, &id);
	bind();
}

Framebuffer::~Framebuffer()
{
	glDeleteFramebuffersEXT(1, &id);
}

void Framebuffer::bind() const
{
	if(!cur_fbo)
		get(GL_VIEWPORT, sys_viewport);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, id);
	cur_fbo=this;
	if(width && height)
		viewport(0, 0, width, height);
}

void Framebuffer::attach(FramebufferAttachment attch, Renderbuffer &rbuf)
{
	maybe_bind();
	glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, attch, GL_RENDERBUFFER_EXT, rbuf.get_id());
	get_or_create_attachment(attch)=rbuf;
	check_size();
}

void Framebuffer::attach(FramebufferAttachment attch, Texture2D &tex, int level)
{
	maybe_bind();
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, attch, tex.get_target(), tex.get_id(), level);
	get_or_create_attachment(attch)=tex;
	check_size();
}

void Framebuffer::detach(FramebufferAttachment attch)
{
	maybe_bind();
	for(vector<Attachment>::iterator i=attachments.begin(); i!=attachments.end(); ++i)
		if(i->attachment==attch)
		{
			if(i->type==GL_RENDERBUFFER_EXT)
				glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, attch, GL_RENDERBUFFER_EXT, 0);
			else if(i->type==GL_TEXTURE_2D)
				glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, attch, GL_TEXTURE_2D, 0, 0);
			attachments.erase(i);
			check_size();
			return;
		}
}

FramebufferStatus Framebuffer::check_status() const
{
	maybe_bind();
	return static_cast<FramebufferStatus>(glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT));
}

const Framebuffer *Framebuffer::current()
{
	return cur_fbo;
}

void Framebuffer::unbind()
{
	if(cur_fbo)
	{
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
		cur_fbo=0;
		viewport(sys_viewport[0], sys_viewport[1], sys_viewport[2], sys_viewport[3]);
	}
}

void Framebuffer::maybe_bind() const
{
	if(cur_fbo!=this)
		bind();
}

Framebuffer::Attachment &Framebuffer::get_or_create_attachment(FramebufferAttachment attch)
{
	for(vector<Attachment>::iterator i=attachments.begin(); i!=attachments.end(); ++i)
		if(i->attachment==attch)
			return *i;
	attachments.push_back(Attachment(attch));
	return attachments.back();
}

void Framebuffer::check_size()
{
	if(!attachments.empty())
	{
		const Attachment &attch=attachments.front();
		if(attch.type==GL_RENDERBUFFER_EXT)
		{
			width=attch.rbuf->get_width();
			height=attch.rbuf->get_height();
		}
		else if(attch.type==GL_TEXTURE_2D)
		{
			Texture2D *tex=static_cast<Texture2D *>(attch.tex);
			width=tex->get_width();
			height=tex->get_height();
		}
		if(cur_fbo==this)
			viewport(0, 0, width, height);
	}
}

const Framebuffer *Framebuffer::cur_fbo=0;
int Framebuffer::sys_viewport[4]={ 0, 0, 1, 1 };


Framebuffer::Attachment::Attachment(FramebufferAttachment a):
	attachment(a),
	type(0)
{ }

Framebuffer::Attachment &Framebuffer::Attachment::operator=(Renderbuffer &r)
{
	type=GL_RENDERBUFFER_EXT;
	rbuf=&r;
	return *this;
}

Framebuffer::Attachment &Framebuffer::Attachment::operator=(Texture &t)
{
	type=t.get_target();
	tex=&t;
	return *this;
}


void viewport(int x, int y, unsigned w, unsigned h)
{
	glViewport(x, y, w, h);
}

void clear(BufferBits bits)
{
	glClear(bits);
}

void draw_buffer(RWBuffer buf)
{
	glDrawBuffer(buf);
}

void read_buffer(RWBuffer buf)
{
	glReadBuffer(buf);
}

} // namespace GL
} // namespace Msp
