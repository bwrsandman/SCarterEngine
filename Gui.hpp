/* 
 * File:   Gui.hpp
 * Author: sandy
 *
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
 * 
 * Created on February 17, 2013, 11:40 PM
 */

#ifndef GUI_HPP
#define	GUI_HPP

#include <gtkmm.h>
class SceneBase;
class SceneGears; // TODO: TEMP

class Gui 
{
public:
    Gui();
    Gui(int, char**, const char* const);
    Gui(const Gui& orig);
    virtual ~Gui();
    void Draw(void);
private:
    const char* const _main_window = "main_window";
    const char* const _gl_frame = "gl_frame";

    Gtk::Main* kit = NULL;
    Gtk::Window* main_win = NULL;
    Gtk::Alignment* gl_container = NULL;
    Gtk::Scale* scl_norm = NULL;
    SceneBase* scene = NULL;
    SceneGears* gears = NULL;

    void on_set_norm_alpha(void);
    void on_switch_scene_page(GtkNotebookPage*, guint);
};

#endif	/* GUI_HPP */

