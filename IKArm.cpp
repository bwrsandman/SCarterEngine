/* 
 * File:   IKArm.cpp
 * Author: sandy
 * 
 * Created on February 18, 2013, 1:29 PM
 */

#include "IKLine.hpp"
#include "IKArm.hpp"
#include "math3d.hpp"

IKArm::IKArm() 
    : lines(new IKLine[3])
{
    /*************************
     * 
     *    x
     *    |
     *    | C
     *    x
     *    |
     *    | B
     *    x
     *    |
     *    | A
     *    x
     * 
     *************************/
    for (int i = 1; i < 3; ++i)
        lines[i].set_parent(&lines[i-1]);
}

IKArm::IKArm(const IKArm& orig)
{
}

IKArm::~IKArm() {
}

float IKArm::loc_orient(int i) const
{
    return lines[i].loc_orient();
}

float IKArm::len(int i) const 
{ 
    return lines[i].length(); 
}

float IKArm::dXdAlpha() const
{
    return A() * -sin(lines[0].cum_orient()) + dXdBeta();
}

float IKArm::dYdAlpha() const
{
    return A() *  cos(lines[0].cum_orient()) + dYdBeta();
}

float IKArm::dXdBeta() const
{
    return B() * -sin(lines[1].cum_orient()) + dXdGamma();
}

float IKArm::dYdBeta() const
{
    return B() *  cos(lines[1].cum_orient()) + dYdGamma();
}

float IKArm::dXdGamma() const
{
    return C() * -sin(lines[2].cum_orient());
}

float IKArm::dYdGamma() const
{
    return C() * cos(lines[2].cum_orient());
}

Matrix2x3f IKArm::Jacobian() const
{
    return Matrix2x3f( dXdAlpha(), dXdBeta(), dXdGamma(),
                       dYdAlpha(), dYdBeta(), dYdGamma() );
}

Matrix3x2f IKArm::Jtransp() const
{
    return Jacobian().transpose();
}

Matrix2f IKArm::JJtransp() const
{
    return Jacobian() * Jtransp();
}

Matrix2f IKArm::invJJtransp() const
{
    return JJtransp().inverse();
}

Matrix3x2f IKArm::Jplus() const
{
    return Jtransp() * invJJtransp();
}

void IKArm::move_by(Vector2f delta)
{
    Vector3f moved = Jplus() * delta;
    
    lines[0].add_orient(moved.X); // delta alpha
    lines[1].add_orient(moved.Y); // delta beta
    lines[2].add_orient(moved.Z); // delta gamma
    
}

void IKArm::move_to(Vector2f dest, float t)
{
    move_by(max_vel * (dest - lines[2].cum_tip()).normalized() * t);
}