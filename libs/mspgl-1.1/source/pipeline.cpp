/* $Id: pipeline.cpp 84 2009-09-21 13:50:58Z tdb $

This file is part of libmspgl
Copyright © 2009  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include "effect.h"
#include "except.h"
#include "framebuffer.h"
#include "lighting.h"
#include "pipeline.h"
#include "postprocessor.h"
#include "renderbuffer.h"
#include "texture2d.h"

using namespace std;

namespace Msp {
namespace GL {

Pipeline::Pipeline(unsigned w, unsigned h, bool d):
	width(w),
	height(h),
	hdr(d),
	fbo(0),
	color_buf(0),
	depth_buf(0)
{ }

Pipeline::~Pipeline()
{
	delete fbo;
	delete color_buf;
	delete depth_buf;
}

PipelinePass &Pipeline::add_pass(const Tag &tag)
{
	if(passes.count(tag.id))
		throw KeyError("Pass already exists");

	PipelinePass &pass=passes[tag.id];
	pass_order.push_back(tag);
	return pass;
}

PipelinePass &Pipeline::get_pass(const Tag &tag)
{
	map<unsigned, PipelinePass>::iterator i=passes.find(tag.id);
	if(i==passes.end())
		throw KeyError("Unknown pass");
	return i->second;
}

const PipelinePass &Pipeline::get_pass(const Tag &tag) const
{
	map<unsigned, PipelinePass>::const_iterator i=passes.find(tag.id);
	if(i==passes.end())
		throw KeyError("Unknown pass");
	return i->second;
}

bool Pipeline::has_pass(const Tag &tag) const
{
	return passes.count(tag.id);
}

void Pipeline::add_renderable(const Renderable &r)
{
	renderables.push_back(&r);
}

void Pipeline::add_effect(Effect &e)
{
	effects.push_back(&e);
}

void Pipeline::add_postprocessor(PostProcessor &pp)
{
	postproc.push_back(&pp);
	if(!fbo)
	{
		fbo=new Framebuffer;
		color_buf=new Texture2D;
		color_buf->set_min_filter(NEAREST);
		color_buf->set_mag_filter(NEAREST);
		color_buf->storage((hdr ? RGB16F : RGB), width, height, 0);
		color_buf->image(0, RGB, UNSIGNED_BYTE, 0);
		fbo->attach(COLOR_ATTACHMENT0, *color_buf, 0);
		depth_buf=new Renderbuffer;
		depth_buf->storage(DEPTH_COMPONENT, width, height);
		fbo->attach(DEPTH_ATTACHMENT, *depth_buf);
		Framebuffer::unbind();
	}
}

void Pipeline::render(const Tag &tag) const
{
	const PipelinePass &pass=get_pass(tag);
	if(pass.lighting)
		pass.lighting->bind();
	for(vector<Effect *>::const_iterator i=pass.effects.begin(); i!=pass.effects.end(); ++i)
		(*i)->prepare();
	for(vector<const Renderable *>::const_iterator i=renderables.begin(); i!=renderables.end(); ++i)
		(*i)->render(tag);
	for(vector<Effect *>::const_iterator i=pass.effects.end(); i--!=pass.effects.begin();)
		(*i)->cleanup();
	if(pass.lighting)
		Lighting::unbind();
}

void Pipeline::render_all() const
{
	if(fbo)
	{
		fbo->bind();
		clear(COLOR_BUFFER_BIT|DEPTH_BUFFER_BIT);
	}
	for(vector<Effect *>::const_iterator i=effects.begin(); i!=effects.end(); ++i)
		(*i)->prepare();
	for(vector<Tag>::const_iterator i=pass_order.begin(); i!=pass_order.end(); ++i)
		render(*i);
	for(vector<Effect *>::const_iterator i=effects.end(); i--!=effects.begin();)
		(*i)->cleanup();
	if(fbo)
		Framebuffer::unbind();
	// XXX Need two color buffer textures to handle multiple post-processors correctly
	for(vector<PostProcessor *>::const_iterator i=postproc.begin(); i!=postproc.end(); ++i)
		(*i)->render(*color_buf);
}

} // namespace GL
} // namespace Msp
