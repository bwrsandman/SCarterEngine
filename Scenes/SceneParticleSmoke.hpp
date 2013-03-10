/* 
 * File:   SceneParticleSmoke.hpp
 * Author: sandy
 *
 * Created on March 9, 2013, 3:29 PM
 */

#ifndef SCENEPARTICLESMOKE_HPP
#define	SCENEPARTICLESMOKE_HPP

#include <gtkmm.h>
#include <gtkglmm.h>
#include "SceneParticleBase.hpp"

class SceneParticleSmoke : public SceneParticleBase
{
public:
    SceneParticleSmoke();
    SceneParticleSmoke(const SceneParticleSmoke& orig);
    virtual ~SceneParticleSmoke();
private:

};

#endif	/* SCENEPARTICLESMOKE_HPP */

