/* $Id$

This file is part of flipout
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef STATE_H_
#define STATE_H_

class FlipOut;

class State
{
protected:
	FlipOut &flipout;
	bool finished;
	bool opaque;

	State(FlipOut &);
public:
	virtual ~State() { }

	bool get_finished() const { return finished; }
	bool get_opaque() const { return opaque; }

	virtual void enable() { }
	virtual void disable() { }
	virtual void tick() =0;
	virtual void render() const =0;
};

#endif
