/* $Id$

This file is part of flipout
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include <msp/gl/meshbuilder.h>
#include <msp/gl/misc.h>
#include <msp/gl/texture2d.h>
#include <msp/gl/texunit.h>
#include <msp/gl/uniform.h>
#include "background.h"
#include "flipout.h"
#include "geomcategory.h"
#include "playfield.h"
#include "reflect.h"
#include "wall.h"

using namespace std;
using namespace Msp;

Wall::Wall(Playfield &pf, Side s):
	Object(pf),
	side(s),
	mesh((GL::COLOR4_UBYTE, GL::TEXCOORD2, GL::NORMAL3, GL::VERTEX3))
{
	vector<int> m(4);
	switch(side)
	{
	case FLOOR:
		geom=dCreatePlane(playfield.get_space(), 0, 0, 1, 0);
		m[0]=0; m[1]=1; m[2]=3; m[3]=2;
		break;
	case LEFT:
		geom=dCreatePlane(playfield.get_space(), 1, 0, 0, -playfield.get_width()/2);
		m[0]=0; m[1]=2; m[2]=6; m[3]=4;
		break;
	case RIGHT:
		geom=dCreatePlane(playfield.get_space(), -1, 0, 0, -playfield.get_width()/2);
		m[0]=1; m[1]=5; m[2]=7; m[3]=3;
		break;
	case TOP:
		geom=dCreatePlane(playfield.get_space(), 0, -1, 0, -playfield.get_height());
		m[0]=2; m[1]=3; m[2]=7; m[3]=6;
		break;
	}

	vector<Vector3> p(2);
	p[0]=Vector3(-playfield.get_width()/2, 0, 0);
	p[1]=Vector3(playfield.get_width()/2, playfield.get_height(), 2);

	mkquad(&p[0], &m[0]);

	dGeomSetCategoryBits(geom, side==FLOOR ? ::FLOOR : ::WALL);
	dGeomSetCollideBits(geom, 0);
	dGeomSetData(geom, static_cast<Object *>(this));

	playfield.add(*this);
	playfield.get_scene().add(*this);
}

Wall::~Wall()
{
	dGeomDestroy(geom);

	playfield.remove(*this);
	playfield.get_scene().remove(*this);
}

bool Wall::has_pass(const GL::Tag &tag) const
{
	return tag.id==0 || tag.id==GL::Tag("noshader").id || tag.id==GL::Tag("light").id;
}

void Wall::render(const GL::Tag &tag) const
{
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	GL::enable(GL_COLOR_MATERIAL);
	
	const Background *bg=playfield.get_background();
	bg->apply(tag.id!=GL::Tag("noshader").id);
	GL::Program *shader=bg->get_shader();
	if(shader)
	{
		if(tag.id==GL::Tag("light").id)
		{
			GL::Uniform1i(1).apply(shader->get_uniform_location("use_shadow"));
			GL::Uniform1i(2).apply(shader->get_uniform_location("shadow"));
		}
		else
			GL::Uniform1i(0).apply(shader->get_uniform_location("use_shadow"));
	}

	if(side==FLOOR && playfield.get_reflect())
	{
		GL::TexUnit::activate(1);
		playfield.get_reflect()->get_texture()->bind();
	}
	else if(shader)
		GL::Uniform1f(0.0).apply(shader->get_uniform_location("refl_str"));

	mesh.draw();

	if(side==FLOOR && playfield.get_reflect())
	{
		GL::Texture::unbind();
		GL::TexUnit::activate(0);
	}
	GL::Program::unbind();
	GL::Texture::unbind();
	GL::disable(GL_COLOR_MATERIAL);
}

void Wall::mkquad(const Vector3 *p, const int *m)
{
	GL::MeshBuilder bld(mesh);
	bld.begin(GL::QUADS);
	bld.normal(0, 0, 1);
	for(unsigned i=0; i<4; ++i)
	{
		Vector3 q(p[(m[i]&1)!=0].x, p[(m[i]&2)!=0].y, p[(m[i]&4)!=0].z);
		bld.color(1.0f, 1.0f, 1.0f, (q.z==0 ? 1.0f : 0.0f));
		bld.texcoord((q.x<=0 ? 0 : 1), (q.y<=0 ? 0 : playfield.get_height()/playfield.get_width()));
		bld.vertex(q.x, q.y, q.z);
	}
	bld.end();
}
