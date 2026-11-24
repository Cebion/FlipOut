/* $Id: state.h 27 2008-03-03 17:48:55Z tdb $

This file is part of libmspgltk
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_GLTK_STATE_H_
#define MSP_GLTK_STATE_H_

#include <istream>
#include <msp/datafile/value.h>

namespace Msp {
namespace GLtk {

enum State
{
	NORMAL=0,   //< Default state
	HOVER=1,    //< Pointer over the widget
	ACTIVE=2,   //< Widget is active (e.g. pressed button)
	FOCUS=4,    //< Widget has input focus
	DISABLED=8, //< Widget is unresponsive
	N_STATES_=16 //< Sentry value
};

inline State operator|(State a, State b)
{ return static_cast<State>(static_cast<int>(a)|static_cast<int>(b)); }

inline State operator|=(State &a, State b)
{ a=a|b; return a; }

inline State operator&(State a, State b)
{ return static_cast<State>(static_cast<int>(a)&static_cast<int>(b)); }

inline State operator&=(State &a, State b)
{ a=a&b; return a; }

inline State operator~(State a)
{ return static_cast<State>(~static_cast<int>(a)); }

extern std::istream &operator>>(std::istream &, State &);

} // namespace GLtk
} // namespace Msp

#endif
