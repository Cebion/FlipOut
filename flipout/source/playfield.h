/* $Id$

This file is part of flipout
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef PLAYFIELD_H_
#define PLAYFIELD_H_

#include <set>
#include <ode/ode.h>
#include <sigc++/signal.h>
#include <msp/al/soundscape.h>
#include <msp/gl/light.h>
#include <msp/gl/lighting.h>
#include <msp/gl/mesh.h>
#include <msp/gl/scene.h>
#include <msp/gl/texture.h>
#include <msp/time/timedelta.h>
#include "camera.h"
#include "vector3.h"

class Background;
class Ball;
class Bonus;
class BonusAction;
class BonusEffect;
class BonusType;
class Brick;
class Collider;
class FlipOut;
class Glow;
class Object;
class Paddle;
class Shadow;
class Reflect;
class Wall;

/**
The Playfield is in charge of most of the game mechanics.  It is usually
created from a Level, although this is not a requirement.
*/
class Playfield
{
public:
	sigc::signal<void, unsigned> signal_score;
	sigc::signal<bool> signal_lost_life;
	sigc::signal<void> signal_finished;
	sigc::signal<void> signal_extra_life;

private:
	struct ContactInfo
	{
		dGeomID g1;
		dGeomID g2;
		Object *o1;
		Object *o2;
		dContact contacts[4];
		int count;

		ContactInfo(dGeomID a, dGeomID b): g1(a), g2(b) { }
	};

	// General properties
	FlipOut &flipout;
	float width;
	float height;
	float gravity;
	const Background *background;

	// Containers
	dWorldID world;
	dSpaceID space;
	dJointGroupID contacts;
	Msp::GL::Scene scene;
	std::list<Collider *> colliders;

	// Objects and gameplay parameters
	std::set<Object *> objects;
	std::set<Ball *> balls;
	std::set<Bonus *> bonuses;
	unsigned moving_balls;
	unsigned n_bricks;
	bool balls_stuck;
	Paddle *paddle;
	std::list<BonusEffect *> effects;
	Msp::GL::Light light_diffuse;
	Msp::GL::Light light_fill;
	Msp::GL::Light light_combined;
	Msp::GL::Lighting lighting_diffuse;
	Msp::GL::Lighting lighting_fill;
	Msp::GL::Lighting lighting_combined;
	const BonusType *fill_bonus;
	float ball_magnet;
	bool unlimited_balls;
	float gravity_modifier;
	float friction_modifier;
	float quake_strength;
	float quake_phase;
	float quake_scale;

	// Awards
	std::map<std::string, unsigned> awards;
	unsigned juggler_level;
	unsigned bonuses_created;
	unsigned bonuses_collected;

	// Camera and effects
	Camera camera;
	Glow *glow;
	Shadow *shadow;
	Reflect *reflect;

	// Audio
	Msp::AL::SoundScape scape;

	// Debugging data
	unsigned n_contacts;

public:
	Playfield(FlipOut &, float, float);
	~Playfield();

	FlipOut &get_flipout() const { return flipout; }
	float get_width() const { return width; }
	float get_height() const { return height; }
	const Camera &get_camera() const { return camera; }
	dWorldID get_world() { return world; }
	dSpaceID get_space() { return space; }
	Msp::GL::Scene &get_scene() { return scene; }
	const Shadow *get_shadow() const { return shadow; }
	const Reflect *get_reflect() const { return reflect; }
	Msp::AL::SoundScape &get_soundscape() { return scape; }
	const std::map<std::string, unsigned> &get_awards() const { return awards; }
	unsigned get_n_contacts() const { return n_contacts; }
	unsigned get_n_objects() const { return objects.size(); }

	void set_gravity(float);
	void set_gravity_modifier(float);
	float get_gravity() const { return gravity*gravity_modifier; }
	void set_friction_modifier(float);
	void set_background(const Background *);
	const Background *get_background() const { return background; }
	void set_fill_bonus(const BonusType *);
	const BonusType *get_fill_bonus() const { return fill_bonus; }
	void set_ball_magnet(double);
	double get_ball_magnet() const { return ball_magnet; }
	void set_unlimited_balls(bool);
	void set_quake(float);

	void add(Object &);
	void add(Ball &);
	void add(Brick &);
	void add(Paddle &);
	void add(Bonus &);
	void remove(Object &);
	void remove(Ball &);
	void remove(Brick &);
	void remove(Bonus &);
	const std::set<Ball *> &get_balls() const { return balls; }

	Paddle *get_paddle() const { return paddle; }
	void destroy_bonuses();
	void add_effect(const BonusType &, const BonusAction &);
	void place_ball(Ball &);

	void step(const Msp::Time::TimeDelta &);
	void render() const;

private:
	static void near_callback(void *, dGeomID, dGeomID);
	bool collide(ContactInfo &);
	void create_contacts(ContactInfo &);
	void ball_stuck(bool);
	void activate_objects();

	bool collide_ball_brick(Ball &, Brick &, dContact &);
	bool collide_ball_bonus(Ball &, Bonus &, dContact &);
	bool collide_ball_ball(Ball &, Ball &, dContact &);
	bool collide_paddle_bonus(Paddle &, Bonus &, dContact &);
	bool collide_paddle_object(Paddle &, Object &, dContact &);
	bool collide_brick_brick(Brick &, Brick &, dContact &);
	bool collide_brick_bonus(Brick &, Bonus &, dContact &);
	bool collide_wall_object(Wall &, Object &, dContact &);
};

#endif
