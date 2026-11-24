/* $Id: meta.h 50 2008-08-05 06:33:56Z tdb $

This file is part of libmspcore
Copyright © 2008  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_CORE_META_H_
#define MSP_CORE_META_H_

namespace Msp {

template<typename T>
struct RemoveConst
{ typedef T Type; };

template<typename T>
struct RemoveConst<const T>
{ typedef T Type; };

} // namespace Msp

#endif
