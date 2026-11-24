/* $Id: pipeline.h 76 2009-01-27 19:38:22Z tdb $

This file is part of libmspgl
Copyright © 2009  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_GL_PIPELINE_H_
#define MSP_GL_PIPELINE_H_

#include <map>
#include "pipelinepass.h"
#include "renderable.h"

namespace Msp {
namespace GL {

class Effect;
class Framebuffer;
class PostProcessor;
class Renderbuffer;
class Texture2D;

class Pipeline: public Renderable
{
private:
	std::map<unsigned, PipelinePass> passes;
	std::vector<Tag> pass_order;
	std::vector<const Renderable *> renderables;
	std::vector<Effect *> effects;
	std::vector<PostProcessor *> postproc;
	unsigned width;
	unsigned height;
	bool hdr;
	Framebuffer *fbo;
	Texture2D *color_buf;
	Renderbuffer *depth_buf;

public:
	Pipeline(unsigned, unsigned, bool);
	~Pipeline();

	PipelinePass &add_pass(const Tag &tag);
	PipelinePass &get_pass(const Tag &tag);
	const PipelinePass &get_pass(const Tag &tag) const;
	virtual bool has_pass(const Tag &tag) const;

	void add_renderable(const Renderable &);
	void add_effect(Effect &);
	void add_postprocessor(PostProcessor &);

	virtual void render(const Tag &tag=Tag()) const;
	void render_all() const;
};

} // namespace GL
} // namespace Msp

#endif
