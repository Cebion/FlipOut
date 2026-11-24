/* $Id$

This file is part of flipout
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef LEVEL_H_
#define LEVEL_H_

#include <msp/datafile/loader.h>
#include <msp/gl/tag.h>
#include <msp/gl/texture.h>

class BonusType;
class BrickType;
class FlipOut;
class Playfield;
class Resources;

/**
A Level holds data that can be used to create a Playfield.
*/
class Level
{
public:
	struct BrickDef
	{
		class Loader: public Msp::DataFile::Loader
		{
		private:
			BrickDef &bdef;
			Resources &res;

		public:
			typedef Resources Collection;

			Loader(BrickDef &, Resources &);
			BrickDef &get_object() const { return bdef; }
			Resources &get_collection() const { return res; }
		private:
			void angle(float);
		};

		const BrickType *type;
		std::string color;
		float x, y, z;
		float angle;
		const BonusType *bonus;

		BrickDef(const BrickType *);
		void save(std::list<Msp::DataFile::Statement> &, const Resources &) const;
	};

private:
	std::string title;
	float width;
	float height;
	std::list<BrickDef> bricks;
	float gravity;
	float bonus_chance;

public:
	class Loader: public Msp::DataFile::Loader
	{
	private:
		Level &level;
		Resources &res;

	public:
		typedef Resources Collection;

		Loader(Level &, Resources &);
		Level &get_object() const { return level; }
	private:
		void bonus(const std::string &);
		void brick(const std::string &);
	};

	Level();
	void set_title(const std::string &);
	const std::string &get_title() const { return title; }
	void set_size(float, float);
	float get_width() const { return width; }
	float get_height() const { return height; }
	float get_gravity() const { return gravity; }
	float get_bonus_chance() const { return bonus_chance; }
	void add_brick(const BrickDef &);
	void remove_brick(const BrickDef &);
	const std::list<BrickDef> &get_bricks() const { return bricks; }
	Playfield *create_playfield(FlipOut &) const;
	void save(std::list<Msp::DataFile::Statement> &, const Resources &) const;

	void render(const Msp::GL::Tag &) const;

private:
	BonusType &random_bonus_type(const std::list<BonusType *> &) const;
};

#endif
