/* 
 * File:   SceneParticleFirework.cpp
 * Author: sandy
 * 
 * Created on March 9, 2013, 3:30 PM
 */

#include <GL/glew.h>
#include "../math3d.hpp"

#include "SceneParticleFountain.hpp"

SceneParticleFountain::SceneParticleFountain() {
}

SceneParticleFountain::SceneParticleFountain(const SceneParticleFountain& orig) {
}

SceneParticleFountain::~SceneParticleFountain() {
}

bool SceneParticleFountain::create_shaders(const char* vsh, const char* fsh) 
{
    bool ret = SceneParticleBase::create_shaders(VERTEX_SHADER, FRAGMENT_SHADER);
    return ret;
}

void SceneParticleFountain::create_geom()
{
    SceneParticleBase::create_geom();
}