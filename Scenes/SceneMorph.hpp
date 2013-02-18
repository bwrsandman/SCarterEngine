/* 
 * File:   SceneMorph.hpp
 * Author: sandy
 *
 * Created on February 17, 2013, 11:46 PM
 */

#ifndef SCENEMORPH_HPP
#define	SCENEMORPH_HPP

#include <gtkmm.h>
#include <gtkglmm.h>
#include "SceneBase.hpp"

class Matrix4f;

class SceneMorph : public SceneBase 
                 , public Gtk::GL::Widget<SceneMorph>
{
public:
    SceneMorph();
    SceneMorph(const SceneMorph& orig);
    virtual ~SceneMorph();
protected:
	virtual void create_vao (void);
	virtual void render (GLenum);
};

#endif	/* SCENEMORPH_HPP */

