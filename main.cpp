/*
 * main.cpp
 * Copyright (C) 2013 Unknown <mr.sandy.carter@gmail.com>
 *
 * SCarter_Engine is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * SCarter_Engine is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <iostream>
#include <gtkmm.h>
#include <gtkglmm.h>

#include "SimpleScene.h"

int main (int argc, char** argv)
{
	/* Init GTK */
	Gtk::Main kit(argc, argv);

	/* Init GtkGlExtmm */
	Gtk::GL::init(argc, argv);

	/* Query OpenGL extension version. */
	int major, minor;
	Gdk::GL::query_version(major, minor);
	std::cout << "OpenGL extension version - "
			  << major << "." << minor << std::endl;

	/* Instantiate and run the application. */
	Glib::RefPtr<Gtk::Builder> builder;
	try {
		builder = Gtk::Builder::create_from_file("scarter_engine.glade");
	} catch (const Glib::FileError &ex) {
		std::cerr << ex.what() << std::endl;
	}

	Gtk::Window* main_win = NULL;
	builder->get_widget("main_window", main_win);

	SimpleGLScene* draw = new SimpleGLScene();
	draw->show();
	if (main_win)
	{
		main_win->add(*draw);
		kit.run(*main_win);
	}

	return 0;
}
