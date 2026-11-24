/* $Id$

This file is part of flipout
Copyright © 2008  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef EDITOR_H_
#define EDITOR_H_

#include <sigc++/trackable.h>
#include <msp/fs/path.h>
#include <msp/gl/light.h>
#include <msp/gl/lighting.h>
#include <msp/gltk/connector.h>
#include <msp/gltk/dropdown.h>
#include <msp/gltk/entry.h>
#include <msp/gltk/label.h>
#include <msp/gltk/root.h>
#include "camera.h"
#include "state.h"

class BrickType;
class Level;

class Editor: public State, public sigc::trackable
{
private:
	enum EditState
	{
		NORMAL,
		SAVE,
		LOAD,
		NEW,
		PROPS,
		INACTIVE
	};

	Msp::FS::Path level_dir;
	Msp::GLtk::Root *root;
	Msp::GLtk::Dropdown *drp_brick_colors;
	Msp::GLtk::Panel *pnl_filesel;
	Msp::GLtk::Label *lbl_filesel_title;
	Msp::GLtk::List *lst_files;
	Msp::GLtk::Entry *ent_filename;
	Msp::GLtk::Panel *pnl_properties;
	Msp::GLtk::Label *lbl_props_title;
	Msp::GLtk::Entry *ent_width;
	Msp::GLtk::Entry *ent_height;
	Msp::GLtk::Entry *ent_title;
	Level     *level;
	BrickType *sel_btype;
	std::string sel_bcolor;
	float     bx, by, bz;
	float     angle;
	bool      pos_valid;
	Camera    camera;
	EditState state;
	Msp::GL::Light light;
	Msp::GL::Lighting lighting;

	sigc::signal<void, BrickType *> signal_brick_type_selected;

public:
	Editor(FlipOut &);

	virtual void enable();
	virtual void disable();
	virtual void tick();
	virtual void render() const;
private:
	void key_press(unsigned, unsigned, wchar_t);
	void button_press(int, int, unsigned, unsigned);
	void pointer_motion(int, int);
	void exit();
	void play();
	void save();
	void load();
	void properties();
	void new_level();
	void brick_type_selected(unsigned, const std::string &);
	void brick_color_selected(unsigned, const std::string &);
	void populate_filesel();
	void file_selected(unsigned, const std::string &);
	void filesel_cancel();
	void filesel_ok();
	void properties_cancel();
	void properties_ok();
};

#endif
