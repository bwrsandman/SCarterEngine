/* 
 * File:   Gui.cpp
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

#include <iostream>
#include <gtkmm.h>
#include <gtkglmm.h>

#include "Gui.hpp"
#include "Scenes/SceneMorph.hpp"
#include "Scenes/SceneIK.hpp"
#include "Scenes/SceneGears.hpp"
#include "Scenes/SceneCubicContainer.hpp"
#include "Scenes/SceneParticleSmoke.hpp"
#include "Scenes/SceneParticleFirework.hpp"

Gui::Gui() 
{
}

Gui::Gui(int argc, char** argv, const char* ui_filename)
{
    /* Init GTK */
    kit = new Gtk::Main(argc, argv);

    /* Init GtkGlExtmm */
    Gtk::GL::init(argc, argv);

    /* Query OpenGL extension version. */
    int major, minor;
    Gdk::GL::query_version(major, minor);
    std::cout << "GTK OpenGL extension version - "
                      << major << "." << minor << std::endl;

    /* Instantiate and run the application. */
    Glib::RefPtr<Gtk::Builder> builder;
    try {
            builder = Gtk::Builder::create_from_file(ui_filename);
    } catch (const Glib::FileError &ex) {
            std::cerr << ex.what() << std::endl;
    }

    builder->get_widget(_main_window, main_win);
    builder->get_widget(_gl_frame, gl_container);

    Gtk::Button* btn_close = NULL;
    builder->get_widget("btnClose", btn_close);
    if (btn_close)
            btn_close->signal_clicked().connect(&Gtk::Main::quit);
    else
            std::cerr << "WARNING: Could not grab close button." << std::endl;

    builder->get_widget("sclNorm", scl_norm);
    if (scl_norm)
    {
            scl_norm->set_range (0.0, 1.0);
            scl_norm->signal_value_changed().connect(sigc::mem_fun(*this, &Gui::on_set_norm_alpha));
    } else {
            std::cerr << "WARNING: Could not grab normal slider." << std::endl;
    }
    
    Gtk::Notebook* ntb_Scenes = NULL;
    builder->get_widget("ntbScenes", ntb_Scenes);
    if (ntb_Scenes)
        ntb_Scenes->signal_switch_page().connect(sigc::mem_fun(*this, &Gui::on_switch_scene_page));

    on_switch_scene_page(NULL, ntb_Scenes->get_current_page());
    
    scene->show();
}

Gui::Gui(const Gui& orig) 
{
}

Gui::~Gui() 
{
    delete(scene); scene = NULL;
    delete(main_win); main_win = NULL;
    delete(kit); kit = NULL;
}


void Gui::on_set_norm_alpha(void)
{
    if (scl_norm && scene && dynamic_cast<SceneMorph*>(scene))
            dynamic_cast<SceneMorph*>(scene)->set_alpha(scl_norm->get_value());
    else
            std::cerr << "Scene or slider seems to be missing" << std::endl;
}

void Gui::on_switch_scene_page(GtkNotebookPage* page, guint page_num)
{            
    if (!gl_container)
    {
        std::cerr << "WARNING: Could not grab frame for OpenGL." << std::endl;
        return;
    }
    gl_container->remove();
    delete scene; scene = NULL;
    switch(page_num)
    {
    case 0:
        scene = new SceneGears();
        break;
    case 1:
        scene = new SceneCubicContainer();
        break;
    case 2:
        scene = new SceneMorph();
        break;
    case 3:
        scene = new SceneIK();
        break;
    case 4:
        scene = new SceneParticleSmoke();
        break;
    case 5:
        scene = new SceneParticleFirework();
        break;
    }
    gl_container->add(*scene);
    scene->show();
}

void Gui::Draw (void)
{
	if (main_win)
	{
		kit->run(*main_win);
	}
}

/* TODO: INPUT HANDLING
 * Source: http://gtkglextmm.sourcearchive.com/downloads/1.2.0-4ubuntu1/
bool Gears::on_key_press_event(GdkEventKey* event)
{
  GLfloat x, y, z;

  m_GearsScene.get_view_rot(x, y, z);

  switch (event->keyval)
    {
    case GDK_z:
      z += 5.0;
      break;
    case GDK_Z:
      z -= 5.0;
      break;
    case GDK_Up:
      x += 5.0;
      break;
    case GDK_Down:
      x -= 5.0;
      break;
    case GDK_Left:
      y += 5.0;
      break;
    case GDK_Right:
      y -= 5.0;
      break;
    case GDK_Escape:
      Gtk::Main::quit();
      break;
    default:
      return true;
    }

  m_GearsScene.set_view_rot(x, y, z);

  m_GearsScene.invalidate();

  return true;
}*/