/* 
 * File:   SceneIK.hpp
 * Author: sandy
 *
 * Created on February 18, 2013, 7:33 AM
 */

#ifndef SCENEIK_HPP
#define	SCENEIK_HPP

#include <gtkmm.h>
#include <gtkglmm.h>
#include "SceneBase.hpp"

class SceneIK : public SceneBase 
              , public Gtk::GL::Widget<SceneIK>
{
public:
    SceneIK();
    SceneIK(const SceneIK& orig);
    virtual ~SceneIK();
private:

};

#endif	/* SCENEIK_HPP */

