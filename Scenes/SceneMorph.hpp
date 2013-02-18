#ifndef SCENEMORPH_H
#define SCENEMORPH_H

#include <gtkmm.h>
#include <gtkglmm.h>
#include "SceneBase.hpp"

class Matrix4f;

class SceneMorph : public SceneBase 
                 , public Gtk::GL::Widget<SceneMorph>
{

};

#endif
