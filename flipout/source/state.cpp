/* $Id$

This file is part of flipout
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include "state.h"

State::State(FlipOut &fo):
	flipout(fo),
	finished(false),
	opaque(true)
{ }
