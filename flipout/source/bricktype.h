/* $Id$

This file is part of flipout
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef BRICKTYPE_H_
#define BRICKTYPE_H_

#include <msp/al/buffer.h>
#include <msp/datafile/loader.h>
#include <msp/gl/color.h>
#include <msp/gl/object.h>
#include <msp/time/timedelta.h>
#include "colorscheme.h"

class Brick;
class Resources;

class BrickType: public Msp::GL::Object
{
public:
	class Loader: public Msp::GL::Object::Loader
	{
	private:
		BrickType &btype;

	public:
		Loader(BrickType &bt, Collection &c);
		BrickType &get_object() const { return btype; }
	private:
		void size(float w, float h, float d);
		void color_scheme(const std::string &);
		void color(const std::string &);
		void explode(const std::string &);
	};

	struct Explosion
	{
		class Loader: public Msp::DataFile::Loader
		{
		private:
			Explosion &expl;

		public:
			Loader(Explosion &);
			Explosion &get_object() const { return expl; }
		private:
			void timeout(float);
		};

		BrickType *type;
		unsigned count;
		float speed;
		Msp::Time::TimeDelta timeout;

		Explosion();
	};

private:
	float width;
	float height;
	float depth;
	unsigned durability;
	bool indestructible;
	float bouncyness;
	float bonus_chance;
	ColorScheme colors;
	float boost;
	unsigned score;
	Explosion explosion;
	const Msp::AL::Buffer *hit_sound;

public:
	BrickType();
	float get_width() const { return width; }
	float get_height() const { return height; }
	float get_depth() const { return depth; }
	unsigned get_durability() const { return durability; }
	bool is_indestructible() const { return indestructible; }
	float get_bouncyness() const { return bouncyness; }
	float get_bonus_chance() const { return bonus_chance; }
	float get_boost() const { return boost; }
	const BrickColor &get_color(const std::string & =std::string()) const;
	const ColorScheme &get_color_scheme() const { return colors; }
	unsigned get_score() const { return score; }
	const Explosion &get_explosion() const { return explosion; }
	const Msp::AL::Buffer *get_hit_sound() const { return hit_sound; }
};

#endif
