/* 
 * File:   SceneParticleGravity.cpp
 * Author: sandy
 * 
 * Created on March 9, 2013, 3:29 PM
 */

#include "SceneParticleRepelant.hpp"

SceneParticleGravity::SceneParticleGravity() {
}

SceneParticleGravity::SceneParticleGravity(const SceneParticleGravity& orig) {
}

SceneParticleGravity::~SceneParticleGravity() {
}

bool SceneParticleGravity::create_shaders(const char* vsh, const char* fsh) 
{
    bool ret = SceneParticleBase::create_shaders(VERTEX_SHADER, FRAGMENT_SHADER);
    return ret;
}