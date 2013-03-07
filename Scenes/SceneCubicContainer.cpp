/* 
 * File:   SceneCubicContainer.cpp
 * Author: sandy
 * 
 * Created on March 7, 2013, 1:40 AM
 */

#include <gtkmm.h>
#include <gtkglmm.h>

#include "SceneBase.hpp"
#include "../GameStates/CubicGameState.hpp"

#include "SceneCubicContainer.hpp"



SceneCubicContainer::SceneCubicContainer() 
{
    gs = new CubicGameState();
}

SceneCubicContainer::SceneCubicContainer(const SceneCubicContainer& orig) 
{
}

SceneCubicContainer::~SceneCubicContainer() 
{
    delete(gs);
}

void SceneCubicContainer::reset_game ()
{
	CubicGameState* gsb = new CubicGameState(*this->gs);
        delete(gs);
	this->gs = gsb;
}

void SceneCubicContainer::create_light()
{
}

void SceneCubicContainer::create_geom()
{
    
}

void SceneCubicContainer::update(const float dt)
{
    SceneBase::update(dt);
    gs->update(dt);
}

void SceneCubicContainer::render(const float dt)
{
    SceneBase::render(dt);
    gs->draw();
}