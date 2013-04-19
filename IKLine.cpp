/* 
 * File:   IKLine.cpp
 * Author: sandy
 * 
 * Created on February 18, 2013, 8:53 AM
 */

#include "IKLine.hpp"
#include "math3d.hpp"

IKLine::IKLine(float l)
    : vertices{0.0f, 0.0f,   l, 0.0f}
    , indices {0, 1}
    , len(l)
{
}

IKLine::IKLine(const IKLine& orig)
    : vertices{0.0f, 0.0f,   0.0f, LEN}
    , indices {0, 1}
{
}

IKLine::~IKLine() 
{
    parent = NULL;
}

float IKLine::cum_orient() const
{
    return loc_orientation + ((parent) ? parent->cum_orient() : 0.0f);        
}

Vector2f IKLine::cum_base() const
{
    return ((parent) ? parent->cum_tip() : Vector2f(0.0f, 0.0f));        
}

Vector2f IKLine::cum_tip() const
{
    float o = cum_orient();
    float x = cos(o); 
    float y = sin(o);
    return cum_base() + len * Vector2f(x, y);
}