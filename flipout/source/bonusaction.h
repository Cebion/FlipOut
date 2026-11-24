/* $Id$

This file is part of flipout
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef BONUSACTION_H_
#define BONUSACTION_H_

class BallType;
class Bonus;
class BonusType;
class Playfield;

class BonusAction
{
protected:
	BonusAction() { }
public:
	virtual bool run(Playfield &, const Bonus &) const =0;
	virtual void terminate(Playfield &) const { }
	virtual ~BonusAction() { }
};


class ScoreAction: public BonusAction
{
private:
	unsigned score;

public:
	ScoreAction(unsigned s);
	virtual bool run(Playfield &, const Bonus &) const;
};


class NewBallAction: public BonusAction
{
private:
	const BallType &type;

public:
	NewBallAction(const BallType &);
	virtual bool run(Playfield &, const Bonus &) const;
};


class ExtraLifeAction: public BonusAction
{
public:
	virtual bool run(Playfield &, const Bonus &) const;
};


class GravityAction: public BonusAction
{
private:
	float modifier;

public:
	GravityAction(float);
	virtual bool run(Playfield &, const Bonus &) const;
	virtual void terminate(Playfield &) const;
};


class FrictionAction: public BonusAction
{
private:
	float modifier;

public:
	FrictionAction(float);
	virtual bool run(Playfield &, const Bonus &) const;
	virtual void terminate(Playfield &) const;
};


class FillBonusAction: public BonusAction
{
private:
	const BonusType *btype;

public:
	FillBonusAction(const BonusType *);
	virtual bool run(Playfield &, const Bonus &) const;
	virtual void terminate(Playfield &) const;
};


class BallMagnetAction: public BonusAction
{
private:
	float force;

public:
	BallMagnetAction(float);
	virtual bool run(Playfield &, const Bonus &) const;
	virtual void terminate(Playfield &) const;
};


class UnlimitedBallsAction: public BonusAction
{
public:
	virtual bool run(Playfield &, const Bonus &) const;
	virtual void terminate(Playfield &) const;
};


class TextAction: public BonusAction
{
private:
	std::string text;

public:
	TextAction(const std::string &);
	virtual bool run(Playfield &, const Bonus &) const;
};


class QuakeAction: public BonusAction
{
private:
	float strength;

public:
	QuakeAction(float);
	virtual bool run(Playfield &, const Bonus &) const;
	virtual void terminate(Playfield &) const;
};

#endif
