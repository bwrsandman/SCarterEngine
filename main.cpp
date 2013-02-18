/* 
 * File:   main.cpp
 * Author: Sandy Carter
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
 *
 * Created on February 17, 2013, 11:35 PM
 */

/*
 * 
 */

#include "Gui.hpp"

const char* const _ui_file = "scarter_engine.glade";

int main (int argc, char** argv)
{
	Gui gui(argc, argv, _ui_file);
	gui.Draw();

	return 0;
}

