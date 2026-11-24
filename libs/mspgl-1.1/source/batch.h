/* $Id: batch.h 77 2009-05-24 10:56:35Z tdb $

This file is part of libmspgl
Copyright © 2007 Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_GL_BATCH_H_
#define MSP_GL_BATCH_H_

#include <vector>
#include <msp/datafile/loader.h>
#include "primitivetype.h"
#include "types.h"

namespace Msp {
namespace GL {

class Batch
{
public:
	class Loader: public DataFile::Loader
	{
	public:
		Loader(Batch &);
	private:
		Batch &batch;

		void indices(const std::vector<uint> &);
	};

private:
	PrimitiveType type;
	std::vector<uint> indices;
	uint min_index;
	uint max_index;

public:
	Batch(PrimitiveType t);
	Batch &append(uint);
	void append(const std::vector<uint> &);
	unsigned size() const { return indices.size(); }
	const std::vector<uint> &get_indices() const { return indices; }
	void draw() const;
	void draw_with_buffer(unsigned) const;
};

} // namespace GL
} // namespace Msp

#endif
