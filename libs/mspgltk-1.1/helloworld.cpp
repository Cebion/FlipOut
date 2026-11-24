/* $Id: helloworld.cpp 47 2008-10-11 18:27:05Z tdb $

This file is part of libmspgltk
Copyright © 2008  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

/*
A simple graphical Hello World application implemented with mspgltk.
Demonstrates some of the most common widget types.
*/

#include <msp/core/application.h>
#include <msp/core/refptr.h>
#include <msp/gbase/display.h>
#include <msp/gbase/glcontext.h>
#include <msp/gbase/window.h>
#include <msp/gl/blend.h>
#include <msp/gl/matrix.h>
#include <msp/gl/misc.h>
#include <msp/gl/projection.h>
#include <msp/gltk/button.h>
#include <msp/gltk/entry.h>
#include <msp/gltk/label.h>
#include <msp/gltk/panel.h>
#include <msp/gltk/resources.h>
#include <msp/gltk/root.h>

using namespace Msp;

// Application class.  Because it's so much nicer than global variables.
class HelloWorld: public Msp::Application
{
private:
	// Objects for setting up an OpenGL window
	Graphics::Display dpy;
	Graphics::Window wnd;
	Graphics::GLContext glc;

	// GLtk resources and widgets
	GLtk::Resources res;
	RefPtr<GLtk::Root> root;
	GLtk::Entry *ent_name;
	GLtk::Label *lbl_hello;

	// Indicate our main class to the core library
	static Application::RegApp<HelloWorld> reg;

public:
	HelloWorld(int, char **);
private:
	virtual void tick();
	void show_hello();
};

Application::RegApp<HelloWorld> HelloWorld::reg;


HelloWorld::HelloWorld(int, char **):
	wnd(dpy, 200, 200),
	glc(wnd)
{
	wnd.set_title("Hello World");
	wnd.signal_close.connect(sigc::bind(sigc::mem_fun(this, &HelloWorld::exit), 0));

	// Load resources.  This must be done before any widgets are created.
	DataFile::load(res, "basic.skin");

	// A Root receives input from a Graphics::Window and passes it on
	root=new GLtk::Root(res, wnd);

	/* Container widgets will delete their contents upon destruction so we can
	safely forget about the pointers after setting the widgets up. */

	// Panels can be used to divide the window into sub-areas
	GLtk::Panel *panel=new GLtk::Panel(res);
	root->add(*panel);
	panel->set_geometry(GLtk::Geometry(20, 20, 160, 160));

	GLtk::Label *lbl;
	// Prompts can be displayed with Labels
	panel->add(*(lbl=new GLtk::Label(res, "Type your name below:")));
	lbl->set_geometry(GLtk::Geometry(10, 130, 140, 20));

	// The user can type text into an Entry
	panel->add(*(ent_name=new GLtk::Entry(res)));
	ent_name->set_geometry(GLtk::Geometry(10, 110, 140, 20));

	GLtk::Button *btn;
	// Buttons can be wired to cause things to happen
	panel->add(*(btn=new GLtk::Button(res, "Hello")));
	btn->set_geometry(GLtk::Geometry(10, 85, 140, 20));
	btn->signal_clicked.connect(sigc::mem_fun(this, &HelloWorld::show_hello));

	// Another label for displaying some information
	panel->add(*(lbl_hello=new GLtk::Label(res)));
	lbl_hello->set_geometry(GLtk::Geometry(10, 65, 140, 20));

	// The user might want to exit the program (*gasp*)
	panel->add(*(btn=new GLtk::Button(res, "Exit")));
	btn->set_geometry(GLtk::Geometry(50, 10, 100, 20));
	btn->signal_clicked.connect(sigc::bind(sigc::mem_fun(this, &HelloWorld::exit), 0));

	// Font rendering requires blending
	GL::enable(GL::BLEND);
	GL::blend_func(GL::SRC_ALPHA, GL::ONE_MINUS_SRC_ALPHA);

	// Done with setting things up, show the window!
	wnd.show();
}

// This function will be called periodically from the main loop
void HelloWorld::tick()
{
	dpy.tick();

	// Set up an orthogonal projection matching the root widget
	GL::matrix_mode(GL::PROJECTION);
	GL::load_identity();
	GL::ortho_bottomleft(200, 200);
	GL::matrix_mode(GL::MODELVIEW);
	GL::load_identity();

	root->render();

	glc.swap_buffers();
}

// Displays a greeting to the user
void HelloWorld::show_hello()
{
	lbl_hello->set_text("Hello, "+ent_name->get_text()+"!");
}
