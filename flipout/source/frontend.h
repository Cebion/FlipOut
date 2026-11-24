/* $Id$

This file is part of flipout
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef FRONTEND_H_
#define FRONTEND_H_

#include <msp/gltk/button.h>
#include <msp/gltk/connector.h>
#include <msp/gltk/dropdown.h>
#include <msp/gltk/list.h>
#include <msp/gltk/root.h>
#include <msp/gltk/table.h>
#include <msp/gltk/userinterface.h>
#include "state.h"

class FrontEnd: public State, public sigc::trackable
{
public:
	class Connector: public Msp::GLtk::Connector
	{
	private:
		FrontEnd &frontend;

	public:
		Connector(FrontEnd &);
		FrontEnd &get_object() const { return frontend; }
	private:
		void control(Msp::GLtk::Button &, const std::string &);
		void highscore_levels(Msp::GLtk::List &);
		void highscore_table(Msp::GLtk::Table &);
		void levels(Msp::GLtk::List &);
		void resolution(Msp::GLtk::Dropdown &);
		void setting(Msp::GLtk::Widget &, const std::string &);
		void show_panel(Msp::GLtk::Button &, const std::string &);
		void start_series(Msp::GLtk::Button &, const std::string &);
	};

private:
	struct ControlInfo
	{
		std::string name;
		Msp::Input::Control *ctrl;
		Msp::GLtk::Button *btn;
	};

	struct LevelInfo
	{
		Level *level;
		std::string name;
		std::string label;

		bool operator<(const LevelInfo &o) const { return label<o.label; }
	};

	Msp::GLtk::UserInterface *ui;
	Msp::GLtk::Root *root;
	std::list<Msp::GLtk::Panel *> panels;
	std::list<ControlInfo> ctrl_infos;
	std::vector<LevelInfo> levels;
	std::vector<std::string> series;
	LevelInfo *selected_level;
	std::string selected_hs_board;

	sigc::signal<void, const std::string &> signal_highscore_board_selected;

public:
	FrontEnd(FlipOut &);

	void enable();
	void disable();
	void tick() { }
	void render() const;

private:
	void quit();
	void start_series(const std::string &);
	void start_selected_level();
	void show_panel(Msp::GLtk::Panel *);
	void close_panel();
	void capture_control(ControlInfo *);
	void control_captured(ControlInfo *);
	void level_selected(unsigned, const std::string &);
	void resolution_selected(unsigned, const std::string &); 
	void set_fullscreen(bool);
	void set_vertex_buffers(bool);
	void set_music_enabled(bool);
	void set_music_volume(double);
	void save_settings();
	void highscore_level_selected(unsigned, const std::string &);
	void fill_highscore_table(const std::string &, Msp::GLtk::Table *);
	void highscores_changed();
	void start_editor();
};

#endif
