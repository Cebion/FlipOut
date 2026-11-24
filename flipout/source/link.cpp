/* $Id$

This file is part of flipout
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include <msp/gl/immediate.h>
#include <msp/gl/misc.h>
#include "ball.h"
#include "geomcategory.h"
#include "link.h"
#include "playfield.h"

using namespace std;
using namespace Msp;

Link::Link(Playfield &pf, Ball &b1, Ball &b2, double ml):
	Object(pf),
	ball1(b1),
	ball2(b2),
	max_len(ml),
	strength(10)
{
	ball1.set_link(this);
	ball2.set_link(this);

	playfield.add(*this);
	playfield.get_scene().add(*this);
}

Link::~Link()
{
	playfield.remove(*this);
	playfield.get_scene().remove(*this);
}

void Link::destroy()
{
	ball1.set_link(0);
	ball2.set_link(0);
	stale=true;
}

void Link::step(const Time::TimeDelta &)
{
	if(stale)
		return;

	Vector3 p1=ball1.get_position();
	Vector3 p2=ball2.get_position();
	Vector3 delta=p2-p1;

	float dist_sq=dot(delta, delta);
	float dist=sqrt(dist_sq);
	float rad_vel=dot(ball2.get_linear_velocity()-ball1.get_linear_velocity(), delta/dist);
	float force=strength/(dist_sq+1)+rad_vel*0.3/dist;

	ball1.add_force(delta*force);
	ball2.add_force(delta*-force);

	if(dist_sq>max_len*max_len)
		destroy();

	resize_rays(static_cast<unsigned>((max_len-dist)*5/max_len)+1);

	for(vector<Ray>::iterator i=rays.begin(); i!=rays.end(); ++i)
	{
		vector<float> &offs=i->offsets;
		unsigned size=offs.size();
		for(unsigned j=size-1; j>1; j/=2)
			for(unsigned k=j/2; k<size; k+=j)
				offs[k]=(offs[k-j/2]+offs[k+j/2])+(rand()*0.2/RAND_MAX-0.1)*j/size;
	}
}

bool Link::has_pass(const GL::Tag &tag) const
{
	return tag.id==0 || tag.id==GL::Tag("noshader").id || tag.id==GL::Tag("glow").id;
}

void Link::render(const GL::Tag &) const
{
	if(stale)
		return;

	Vector3 p1=ball1.get_position();
	Vector3 p2=ball2.get_position();
	Vector3 delta=p2-p1;
	Vector3 dir=delta/sqrt(dot(delta, delta));
	float xy_len=sqrt(dir.x*dir.x + dir.y*dir.y);
	Vector3 up(dir.x*dir.z/xy_len, dir.y*dir.z/xy_len, xy_len);
	Vector3 side(-dir.y/xy_len, dir.x/xy_len, 0);

	GL::disable(GL_LIGHTING);

	GL::Immediate imm((GL::COLOR4_UBYTE, GL::VERTEX3));
	imm.color(1.0f, 0.0f, 0.0f);
	for(vector<Ray>::const_iterator i=rays.begin(); i!=rays.end(); ++i)
	{
		Vector3 d=delta/(i->offsets.size()-1);
		float c=cos(i->angle);
		float s=sin(i->angle);
		imm.begin(GL::LINE_STRIP);
		for(unsigned j=0; j<i->offsets.size(); ++j)
		{
			Vector3 p=p1+d*j+(up*c+side*s)*i->offsets[j];
			imm.vertex(p.x, p.y, p.z);
		}
		imm.end();
	}

	GL::enable(GL_LIGHTING);
}

void Link::resize_rays(unsigned n)
{
	rays.resize(n);
	for(unsigned i=0; i<n; ++i)
	{
		rays[i].angle=M_PI*i/n;
		rays[i].offsets.resize(9);
	}
}
