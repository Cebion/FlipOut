/* $Id: bloom.cpp 82 2009-07-26 19:06:34Z tdb $

This file is part of libmspgl
Copyright © 2009  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include <cmath>
#include <msp/strings/formatter.h>
#include "blend.h"
#include "bloom.h"
#include "meshbuilder.h"
#include "misc.h"
#include "tests.h"
#include "texunit.h"

using namespace std;

namespace {

static const char blur_vs[]=
	"varying vec2 texcoord;\n"
	"void main()\n"
	"{\n"
	"	gl_Position=vec4(gl_Vertex.xy*2.0-1.0, 0.0, 1.0);\n"
	"	texcoord=gl_Vertex.xy;\n"
	"}";

static const char blur_fs[]=
	"uniform sampler2D source;\n"
	"uniform vec2 delta;\n"
	"uniform float factors[19];\n"
	"uniform int size;\n"
	"varying vec2 texcoord;\n"
	"void main()\n"
	"{\n"
	"	gl_FragColor=vec4(0.0, 0.0, 0.0, 0.0);\n"
	"	for(int i=-size; i<=size; ++i)\n"
	"		gl_FragColor+=texture2D(source, texcoord+delta*i)*factors[i+size];\n"
	"}";

static const char combine_vs[]=
	"varying vec2 texcoord;\n"
	"void main()\n"
	"{\n"
	"	gl_Position=vec4(gl_Vertex.xy*2.0-1.0, 0.0, 1.0);\n"
	"	texcoord=gl_Vertex.xy;\n"
	"}";

static const char combine_fs[]=
	"uniform sampler2D source;\n"
	"uniform sampler2D blurred;\n"
	"uniform float strength;\n"
	"varying vec2 texcoord;\n"
	"void main()\n"
	"{\n"
	"	gl_FragColor=mix(texture2D(source, texcoord), texture2D(blurred, texcoord), strength);\n"
	"}";

}

namespace Msp {
namespace GL {

Bloom::Bloom(unsigned w, unsigned h):
	blur_shader(blur_vs, blur_fs),
	combine_shader(combine_vs, combine_fs),
	quad(VERTEX2)
{
	int loc=blur_shader.get_uniform_location("delta");
	blur_shdata[0].uniform(loc, 1.0f/w, 0.0f);
	blur_shdata[1].uniform(loc, 0.0f, 1.0f/h);

	loc=blur_shader.get_uniform_location("source");
	for(unsigned i=0; i<2; ++i)
	{
		blur_shdata[i].uniform(loc, 0);
		tex[i].storage(RGB16F, w, h, 0);
		tex[i].image(0, RGB, UNSIGNED_BYTE, 0);
		tex[i].set_min_filter(NEAREST);
	}

	combine_shdata.uniform(combine_shader.get_uniform_location("source"), 1);
	combine_shdata.uniform(combine_shader.get_uniform_location("blurred"), 0);

	set_radius(2.0f);
	set_strength(0.2f);

	MeshBuilder mbld(quad);
	mbld.begin(QUADS);
	mbld.vertex(0, 0);
	mbld.vertex(1, 0);
	mbld.vertex(1, 1);
	mbld.vertex(0, 1);
	mbld.end();
}

void Bloom::set_radius(float r)
{
	if(r<=0.0f)
		throw InvalidParameterValue("Radius must be positive");

	int size=min(static_cast<int>(r*3.0f), 9);
	int loc=blur_shader.get_uniform_location("size");
	blur_shdata[0].uniform(loc, size);
	blur_shdata[1].uniform(loc, size);

	vector<float> factors(size*2+1);
	float sum=0.0f;
	r=2*r*r;
	for(int i=-size; i<=size; ++i)
		sum+=(factors[size+i]=exp(-i*i/r));

	for(int i=0; i<=size*2; ++i)
	{
		loc=blur_shader.get_uniform_location(format("factors[%d]", i));
		float f=factors[i]/sum;
		blur_shdata[0].uniform(loc, f);
		blur_shdata[1].uniform(loc, f);
	}
}

void Bloom::set_strength(float s)
{
	if(s<0.0f || s>1.0f)
		throw InvalidParameterValue("Strength must be in the range [0.0, 1.0]");
	combine_shdata.uniform(combine_shader.get_uniform_location("strength"), s);
}

void Bloom::render(const Texture2D &src)
{
	const Framebuffer *dest=Framebuffer::current();
	blur_shader.bind();
	fbo.bind();
	src.bind_to(0);
	disable(DEPTH_TEST);
	disable(BLEND);
	for(unsigned i=0; i<2; ++i)
	{
		fbo.attach(COLOR_ATTACHMENT0, tex[i], 0);
		blur_shdata[i].apply();
		quad.draw();
		tex[i].bind_to(0);
	}

	if(dest)
		dest->bind();
	else
		Framebuffer::unbind();

	combine_shader.bind();
	combine_shdata.apply();
	src.bind_to(1);
	quad.draw();
	Program::unbind();
	Texture::unbind();
	TexUnit::activate(0);
	Texture::unbind();
	// XXX Should check if the modes were enabled in the first place
	enable(DEPTH_TEST);
	enable(BLEND);
}

} // namespace GL
} // namespace Msp
