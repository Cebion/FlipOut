/* $Id$

This file is part of flipout
Copyright © 2008  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef HIGHSCORES_H_
#define HIGHSCORES_H_

#include <map>
#include <set>
#include <string>
#include <msp/datafile/loader.h>
#include <msp/fs/path.h>
#include <msp/time/timestamp.h>

class ScoreBoard
{
public:
	class Loader: public Msp::DataFile::Loader
	{
	private:
		ScoreBoard &board;

	public:
		Loader(ScoreBoard &);
	private:
		void entry(unsigned, unsigned, const std::string &, Msp::Time::RawTime);
	};

	struct Entry
	{
		unsigned score;
		unsigned level;
		std::string name;
		Msp::Time::TimeStamp time;

		bool operator<(const Entry &) const;
	};

private:
	std::set<Entry> scores;

public:
	const std::set<Entry> &get_scores() const { return scores; }
	void add_score(unsigned, unsigned, const std::string &);
	void truncate(unsigned);
};

class HighScores
{
public:
	class Loader: public Msp::DataFile::Loader
	{
	private:
		HighScores &hs;

	public:
		Loader(HighScores &);
	private:
		void board(const std::string &);
	};

	sigc::signal<void> signal_scores_changed;

private:
	Msp::FS::Path filename;
	std::map<std::string, ScoreBoard> boards;
	unsigned max_entries;

public:
	HighScores();

	const ScoreBoard &get_board(const std::string &) const;
	unsigned get_ranking(const std::string &, unsigned);
	void add_score(const std::string &, unsigned, unsigned, const std::string &);
	void load();
	void save() const;
};

#endif
