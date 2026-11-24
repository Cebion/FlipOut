/* $Id$

This file is part of flipout
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef AWARD_H_
#define AWARD_H_

#include <msp/datafile/loader.h>

class Award
{
public:
	class Loader: public Msp::DataFile::Loader
	{
	private:
		Award &award;
	
	public:
		Loader(Award &);
	private:
		void level(unsigned, unsigned);
	};

private:
	std::map<unsigned, unsigned> levels;

public:
	unsigned get_score(unsigned) const;
};

#endif
