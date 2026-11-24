/* $Id: uniform.h 63 2008-09-24 16:17:29Z tdb $

This file is part of libmspgl
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_GL_UNIFORM_H_
#define MSP_GL_UNIFORM_H_

namespace Msp {
namespace GL {

class Program;

class Uniform
{
protected:
	Uniform() { }
public:
	virtual ~Uniform() { }

	virtual void apply(int) const =0;
};


class Uniform1i: public Uniform
{
private:
	int v;

public:
	Uniform1i(int v_);
	virtual void apply(int index) const;
};


class Uniform1f: public Uniform
{
private:
	float v;

public:
	Uniform1f(float v_);
	virtual void apply(int index) const;
};


class Uniform2f: public Uniform
{
private:
	float v[2];

public:
	Uniform2f(float v0, float v1);
	virtual void apply(int index) const;
};


class Uniform3f: public Uniform
{
private:
	float v[3];

public:
	Uniform3f(float v0, float v1, float v2);
	virtual void apply(int index) const;
};


class Uniform4f: public Uniform
{
private:
	float v[4];

public:
	Uniform4f(float v0, float v1, float v2, float v3);
	virtual void apply(int index) const;
};

} // namespace GL
} // namespace Msp

#endif
