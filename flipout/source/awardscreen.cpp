/* $Id$

This file is part of flipout
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include <cmath>
#include <msp/gltk/userinterface.h>
#include <msp/time/units.h>
#include <msp/time/utils.h>
#include "awardscreen.h"
#include "flipout.h"

using namespace std;
using namespace Msp;

AwardScreen::AwardScreen(FlipOut &fo, unsigned s, const map<string, unsigned> &a):
	State(fo),
	total_score(s),
	total_awards(0),
	award_score(0)
{
	opaque=false;

	GLtk::UserInterface *ui=flipout.get_resources().get<GLtk::UserInterface>("game");

	ui->get_widget("pnl_awards", pnl_awards);
	ui->get_widget("lbl_total_score", lbl_total_score);
	lbl_total_score->set_text(lexical_cast(total_score));

	GLtk::Label *lbl_name;
	ui->get_widget("lbl_award_name", lbl_name);

	GLtk::Geometry name_geom=lbl_name->get_geometry();
	GLtk::Geometry score_geom=lbl_total_score->get_geometry();

	name_geom.y+=name_geom.h*a.size();
	score_geom.y=name_geom.y;

	panel_geom=pnl_awards->get_geometry();
	unsigned h=panel_geom.h+name_geom.h*a.size();
	pnl_awards->set_size(panel_geom.w, h);
	pnl_awards->set_position(panel_geom.x, (ui->get_root().get_geometry().h-h)/2);

	const GLtk::Resources &ui_res=flipout.get_resources().get_ui_resources();
	for(map<string, unsigned>::const_iterator i=a.begin(); i!=a.end(); ++i)
	{
		AwardInfo ai;
		ai.score=i->second;

		ai.lbl_name=new GLtk::Label(ui_res, i->first);
		ai.lbl_name->set_geometry(name_geom);
		pnl_awards->add(*ai.lbl_name);

		ai.lbl_score=new GLtk::Label(ui_res, lexical_cast(i->second));
		ai.lbl_score->set_style("number");
		ai.lbl_score->set_geometry(score_geom);
		pnl_awards->add(*ai.lbl_score);

		awards.push_back(ai);

		name_geom.y-=25;
		score_geom.y=name_geom.y;
	}

	cur_award=awards.end();
	flipout.add_timer(Msp::Time::sec).signal_timeout.connect(sigc::mem_fun(this, &AwardScreen::start));

	flipout.get_controls().exit.signal_press.connect(sigc::hide_return(sigc::mem_fun(this, &AwardScreen::finish)));
}

AwardScreen::~AwardScreen()
{
	for(list<AwardInfo>::iterator i=awards.begin(); i!=awards.end(); ++i)
	{
		delete i->lbl_name;
		delete i->lbl_score;
	}

	pnl_awards->set_geometry(panel_geom);
}

void AwardScreen::tick()
{
	Msp::Time::TimeStamp t=Msp::Time::now();
	Msp::Time::TimeDelta dt=t-last_tick;
	last_tick=t;

	if(cur_award!=awards.end() && cur_award->score>0)
	{
		unsigned ds=static_cast<unsigned>(dt/Msp::Time::sec*speed);
		if(ds<cur_award->score)
		{
			total_score+=ds;
			cur_award->score-=ds;
		}
		else
		{
			total_score+=cur_award->score;
			cur_award->score=0;
		}

		cur_award->lbl_score->set_text(lexical_cast(cur_award->score));
		lbl_total_score->set_text(lexical_cast(total_score));

		if(cur_award->score==0)
			flipout.add_timer(0.5*Msp::Time::sec).signal_timeout.connect(sigc::mem_fun(this, &AwardScreen::next));
	}
}

void AwardScreen::render() const
{
	pnl_awards->render();
}

bool AwardScreen::start()
{
	cur_award=awards.begin();
	speed=sqrt(cur_award->score)*100;
	return false;
}

bool AwardScreen::next()
{
	++cur_award;
	if(cur_award==awards.end())
		flipout.add_timer(Msp::Time::sec).signal_timeout.connect(sigc::mem_fun(this, &AwardScreen::finish));
	else
		speed=sqrt(cur_award->score)*100;
	return false;
}

bool AwardScreen::finish()
{
	finished=true;
	return false;
}
