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
#include "Scenes/SceneParticleRepelant.hpp"
#include "Scenes/SceneParticleFountain.hpp"
#include "Scenes/SceneMD5.hpp"

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
        scl_norm->signal_value_changed().connect(
                    sigc::mem_fun(*this, &Gui::on_set_norm_alpha));
    } else {
            std::cerr << "WARNING: Could not grab normal slider." << std::endl;
    }
    
    builder->get_widget("sclAFPS", scl_afps);
    if (scl_afps)
    {
        scl_afps->set_range (1.0, 60.0);
        scl_afps->set_value(24.0);
        scl_afps->signal_value_changed().connect(
                    sigc::mem_fun(*this, &Gui::on_set_afps));
    } else {
            std::cerr << "WARNING: Could not grab normal slider." << std::endl;
    }
        
    builder->get_widget("sclScale", scl_scale);
    if (scl_scale)
    {
        scl_scale->set_range (0.0, 60.0);
        scl_scale->set_value(30.0);
        scl_scale->signal_value_changed().connect(
                    sigc::mem_fun(*this, &Gui::on_set_scale));
    } else {
            std::cerr << "WARNING: Could not grab normal slider." << std::endl;
    }
    
    builder->get_widget("chkSlerp", chk_slerp);
    if (chk_slerp)
    {
        chk_slerp->set_active(false);
        chk_slerp->signal_toggled().connect(
                    sigc::mem_fun(*this, &Gui::on_toggle_slerp));
    } else {
            std::cerr << "WARNING: Could not grab checkbox." << std::endl;
    }
    
    builder->get_widget("chkJoints", chk_joints);
    if (chk_joints)
    {
        chk_joints->set_active(false);
        chk_joints->signal_toggled().connect(
                    sigc::mem_fun(*this, &Gui::on_toggle_joints));
    } else {
            std::cerr << "WARNING: Could not grab checkbox." << std::endl;
    }
    
    builder->get_widget("chkWireframe", chk_wireframe);
    if (chk_wireframe)
    {
        chk_wireframe->set_active(true);
        chk_wireframe->signal_toggled().connect(
                    sigc::mem_fun(*this, &Gui::on_toggle_wireframe));
    } else {
            std::cerr << "WARNING: Could not grab checkbox." << std::endl;
    }
    
    builder->get_widget("chkFill", chk_fill);
    if (chk_fill)
    {
        chk_fill->set_active(true);
        chk_fill->signal_toggled().connect(
                    sigc::mem_fun(*this, &Gui::on_toggle_fill));
    } else {
            std::cerr << "WARNING: Could not grab checkbox." << std::endl;
    }
    
    Gtk::Notebook* ntb_Scenes = NULL;
    builder->get_widget("ntbScenes", ntb_Scenes);
    if (ntb_Scenes)
        ntb_Scenes->signal_switch_page().connect(
                sigc::mem_fun(*this, &Gui::on_switch_scene_page));

    on_switch_scene_page(NULL, ntb_Scenes->get_current_page());
    
    builder->get_widget("btnLoadMD5Mesh", btn_LoadMD5Mesh);
    if (btn_LoadMD5Mesh)
    {
            btn_LoadMD5Mesh->signal_clicked().connect(
                        sigc::mem_fun(*this, &Gui::on_load_md5_button_clicked));
    } else {
            std::cerr << "WARNING: Could not grab load MD5Mesh Button.\n";
    }
    
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

void Gui::on_set_afps(void)
{
    if (scl_afps && scene && dynamic_cast<SceneMD5*>(scene))
            dynamic_cast<SceneMD5*>(scene)->set_afps(scl_afps->get_value());
    else
            std::cerr << "Scene or slider seems to be missing" << std::endl;
}

void Gui::on_set_scale(void)
{
    if (scl_scale && scene && dynamic_cast<SceneMD5*>(scene))
            dynamic_cast<SceneMD5*>(scene)->set_scale(scl_scale->get_value());
    else
            std::cerr << "Scene or slider seems to be missing" << std::endl;
}

void Gui::on_toggle_slerp(void)
{
    if (chk_slerp && scene && dynamic_cast<SceneMD5*>(scene))
            dynamic_cast<SceneMD5*>(scene)->toggle_slerp(chk_slerp->get_active());
    else
            std::cerr << "Checkbox seems to be missing" << std::endl;
}

void Gui::on_toggle_joints()
{
    if (chk_joints && scene && dynamic_cast<SceneMD5*>(scene))
            dynamic_cast<SceneMD5*>(scene)->toggle_joints(chk_joints->get_active());
    else
            std::cerr << "Checkbox seems to be missing" << std::endl;
}

void Gui::on_toggle_wireframe()
{
    if (chk_wireframe && scene && dynamic_cast<SceneMD5*>(scene))
            dynamic_cast<SceneMD5*>(scene)->toggle_wireframe(chk_wireframe->get_active());
    else
            std::cerr << "Checkbox seems to be missing" << std::endl;
}

void Gui::on_toggle_fill()
{
    if (chk_fill && scene && dynamic_cast<SceneMD5*>(scene))
            dynamic_cast<SceneMD5*>(scene)->toggle_fill(chk_fill->get_active());
    else
            std::cerr << "Checkbox seems to be missing" << std::endl;
}

void Gui::on_load_md5_button_clicked()
{
    if(cur_page != 6)
        return;
    Gtk::FileChooserDialog dialog(*main_win,
        "Please choose a folder",
        Gtk::FILE_CHOOSER_ACTION_OPEN);

    // Add response buttons the the dialog:
    dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
    dialog.add_button("Load", Gtk::RESPONSE_OK);
    
    // Md5 file filter
    Gtk::FileFilter filter_md5;
    filter_md5.set_name("MD5 Mesh");
    filter_md5.add_pattern("*.md5mesh");
    dialog.add_filter(filter_md5);

    int result = dialog.run();

    if(result == Gtk::RESPONSE_OK)
    {
        const char* filename = dialog.get_filename().c_str();
        dynamic_cast<SceneMD5*>(scene)->load_md5_mesh(filename);
    }
}

void Gui::on_switch_scene_page(GtkNotebookPage* page, guint page_num)
{            
    if (!gl_container) {
        std::cerr << "WARNING: Could not grab frame for OpenGL." << std::endl;
        return;
    }
    gl_container->remove();
    delete scene; scene = NULL;
    cur_page = page_num;
    switch(page_num) {
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
        scene = new SceneParticleGravity();
        break;
    case 5:
        scene = new SceneParticleFountain();
        break;
    case 6:
        scene = new SceneMD5();
        break;
    }
    if(scene) {
        gl_container->add(*scene);
        scene->show();
    }
}

void Gui::Draw (void)
{
	if (main_win)
	{
		kit->run(*main_win);
	}
}