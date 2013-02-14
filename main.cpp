/*
 * main.cpp
 * Copyright (C) 2013 Unknown <mr.sandy.carter@gmail.com>
 * 
 * SCarter_engine is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * SCarter_engine is distributed in the hope that it will be useful, but
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

#include "Application.h"

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
	App simple;
	kit.run(simple);

	return 0;
}
