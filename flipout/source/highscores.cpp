/* $Id$

This file is part of flipout
Copyright © 2008  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include <msp/core/except.h>
#include <msp/datafile/writer.h>
#include <msp/fs/dir.h>
#include <msp/fs/stat.h>
#include <msp/fs/utils.h>
#include <msp/io/except.h>
#include <msp/time/utils.h>
#include "highscores.h"

using namespace std;
using namespace Msp;

void ScoreBoard::add_score(unsigned score, unsigned level, const string &name)
{
	Entry entry;
	entry.score=score;
	entry.level=level;
	entry.name=name;
	entry.time=Time::now();

	scores.insert(entry);
}

void ScoreBoard::truncate(unsigned n)
{
	if(scores.size()<=n)
		return;

	set<Entry>::iterator i=scores.begin();
	advance(i, n);
	while(i!=scores.end())
		scores.erase(i++);
}


ScoreBoard::Loader::Loader(ScoreBoard &b):
	board(b)
{
	add("entry", &Loader::entry);
}

void ScoreBoard::Loader::entry(unsigned score, unsigned level, const string &name, Time::RawTime time)
{
	Entry ent;
	ent.score=score;
	ent.level=level;
	ent.name=name;
	ent.time=Time::TimeStamp(time);

	board.scores.insert(ent);
}


bool ScoreBoard::Entry::operator<(const Entry &e) const
{
	return (score>e.score || (score==e.score && time<e.time));
}


HighScores::HighScores():
	filename(FS::get_user_data_dir("flipout")/"score"),
	max_entries(10)
{ }

const ScoreBoard &HighScores::get_board(const string &id) const
{
	map<string, ScoreBoard>::const_iterator i=boards.find(id);
	if(i==boards.end())
		throw KeyError("Unknown board", id);
	return i->second;
}

unsigned HighScores::get_ranking(const string &id, unsigned score)
{
	map<string, ScoreBoard>::const_iterator i=boards.find(id);
	if(i==boards.end())
		return 1;
	const set<ScoreBoard::Entry> &scores=i->second.get_scores();
	unsigned n=1;
	for(set<ScoreBoard::Entry>::const_iterator j=scores.begin(); (j!=scores.end() && j->score>score); ++j)
		++n;
	return n;
}

void HighScores::add_score(const string &id, unsigned score, unsigned level, const string &name)
{
	// XXX Add locking
	load();
	ScoreBoard &brd=boards[id];
	brd.add_score(score, level, name);
	if(max_entries>0)
		brd.truncate(max_entries);
	save();

	signal_scores_changed.emit();
}

void HighScores::load()
{
	if(FS::is_reg(filename))
		DataFile::load(*this, filename.str());
}

void HighScores::save() const
{
	FS::Path dir=FS::dirname(filename);
	if(!exists(dir))
		FS::mkdir(dir, 0755);

	IO::BufferedFile file(filename.str(), IO::M_WRITE);
	DataFile::Writer writer(file);

	for(map<string, ScoreBoard>::const_iterator i=boards.begin(); i!=boards.end(); ++i)
	{
		DataFile::Statement board_st("board");
		board_st.append(i->first);

		const set<ScoreBoard::Entry> scores=i->second.get_scores();
		for(set<ScoreBoard::Entry>::const_iterator j=scores.begin(); j!=scores.end(); ++j)
			board_st.sub.push_back((DataFile::Statement("entry"), j->score, j->level, j->name, j->time.raw()));

		writer.write(board_st);
	}
}

HighScores::Loader::Loader(HighScores &h):
	hs(h)
{
	add("board", &Loader::board);
}

void HighScores::Loader::board(const string &bname)
{
	ScoreBoard &brd=hs.boards[bname];
	load_sub(brd);
}
