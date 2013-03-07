/* 
 * File:   CubicGameState.cpp
 * Author: sandy
 * 
 * Created on March 7, 2013, 3:12 PM
 */

#include "../GL1_Primitives/GL1_Sphere.hpp"
#include "../GL1_Primitives/GL1_Cube.hpp"
#include "CubicGameState.hpp"

CubicGameState::CubicGameState()
    : _ball(new GL1_Sphere(sides/20.0f))
    , _box(new GL1_Cube(sides))
{
}

CubicGameState::CubicGameState(const CubicGameState& orig)
    : CubicGameState()
{
    _running = orig.running();
    _paused  = orig.paused();
    _gravity = orig.gravity();
    _drag    = orig.drag();
    _kenergy = orig.kenergy();
}

CubicGameState::~CubicGameState() 
{
    delete(_ball);
    delete(_box);
}

bool CubicGameState::collidesInX(GL1_Sphere& sphere, GL1_Cube& cube) const
{
    return sphere.left() < cube.left() || sphere.right() > cube.right();
}

bool CubicGameState::collidesInY(GL1_Sphere& sphere, GL1_Cube& cube) const
{
    return sphere.top() > cube.top() || sphere.bottom() < cube.bottom();
}

bool CubicGameState::collidesInZ(GL1_Sphere& sphere, GL1_Cube& cube) const
{
    return sphere.front() > cube.front() || sphere.back() < cube.back();
}

void CubicGameState::update (float dt)
{
    if(_paused)
            return;

    _ball->clear_forces();
    if(_gravity)
            _box->apply_gravity(*_ball);
    if(_drag)
            _box->apply_drag(*_ball);
    _ball->update(dt);
    if (collidesInX(*_ball, *_box))
            _ball->change_direction(0, dt, _kenergy);
    if (collidesInY(*_ball, *_box))
            _ball->change_direction(1, dt, _kenergy);
    if (collidesInZ(*_ball, *_box))
            _ball->change_direction(2, dt, _kenergy);

}
void CubicGameState::draw (void)
{
        _box->draw();
        _ball->draw();
}
