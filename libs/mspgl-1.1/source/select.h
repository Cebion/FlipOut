/* $Id: select.h 52 2008-07-19 11:19:01Z tdb $

This file is part of libmspgl
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_GL_SELECT_H_
#define MSP_GL_SELECT_H_

#include <vector>
#include "types.h"

namespace Msp {
namespace GL {
		
struct SelectRecord
{
	uint min_depth;
	uint max_depth;
	std::vector<uint> names;
};

void select_buffer(std::vector<SelectRecord> &);
void init_names();
void push_name(uint);
void pop_name();
void load_name(uint);

void parse_select_records(const uint *buf, uint, std::vector<SelectRecord> &);

void _parse_internal_select_records(uint);

} // namespace GL
} // namespace Msp

#endif
