/* $Id$

This file is part of flipout
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef SERIES_H_
#define SERIES_H_

#include <vector>
#include <msp/datafile/collection.h>

class Level;

class Series
{
public:
	class Loader: public Msp::DataFile::Loader
	{
	public:
		typedef Msp::DataFile::Collection Collection;

	private:
		Series &series;
		Collection &coll;

	public:
		Loader(Series &, Collection &);

	private:
		void level(const std::string &);
	};

private:
	std::vector<const Level *> levels;

public:
	unsigned get_n_levels() const { return levels.size(); }
	const Level &get_level(unsigned) const;
};

#endif
