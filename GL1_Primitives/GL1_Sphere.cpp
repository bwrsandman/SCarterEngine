/* 
 * File:   GL1_Sphere.cpp
 * Author: sandy
 * 
 * Created on March 7, 2013, 3:27 PM
 */
#include <GL/gl.h>
#include <GL/glu.h>
#include <assert.h>
#include <math.h>

#include "GL1_Sphere.hpp"
#include "../interpolation.hpp"

void drawSphere(double r, int lats, int longs)
{
	glLineWidth( 3.0 );
	glColor3f( 1, 1, 1);

	glBegin( GL_LINE_LOOP );/// don't workglPointSize( 0.0 );
	GLUquadricObj *quadric;
	quadric = gluNewQuadric();

	gluQuadricDrawStyle(quadric, GLU_FILL );
	gluSphere( quadric , r, lats, longs );

	gluDeleteQuadric(quadric); 
	glEndList();

	glEnd();
}

GL1_Sphere::GL1_Sphere(float radius, int lats, int longs, 
                       float x, float y, float z)
        : _radius(radius)
        , _lats(lats)
        , _longs(longs)
        , _px(x)
        , _py(y)
        , _pz(z)
{ }

GL1_Sphere::GL1_Sphere(float radius, float x, float y, float z)
    : GL1_Sphere(radius, 20, 20, x, y, z)
{ }

GL1_Sphere::GL1_Sphere(float radius, Point p)
    : GL1_Sphere(radius, p.x(), p.y(), p.z())
{ }

GL1_Sphere::GL1_Sphere(const GL1_Sphere& orig) {
}

GL1_Sphere::~GL1_Sphere() 
{
}

float GL1_Sphere::speed(void) const  
{ 
    return sqrt(_vx * _vx + _vy * _vy + _vz * _vz); 
}

void GL1_Sphere::put_at(Point p)
{
    _px = p.x(); 
    _py = p.y(); 
    _pz = p.z(); 
}

void GL1_Sphere::draw(void)
{
    glPushMatrix();

    glTranslatef(_px, _py, _pz);
    drawSphere(_radius, _lats, _longs);

    glPopMatrix();
}

void GL1_Sphere::update(float dt)
{
    float ax = _fx / _mass;
    float ay = _fy / _mass;
    float az = _fz / _mass;
    _vx += ax * dt;
    _vy += ay * dt;
    _vz += az * dt;
    _px += _vx * dt;
    _py += _vy * dt;
    _pz += _vz * dt;
}

void GL1_Sphere::clear_forces (void)
{
    _fx =  0.0f;
    _fy =  0.0f;
    _fz =  0.0f;
}

void GL1_Sphere::change_direction(unsigned char dir, float dt, bool kenergy)
{
    assert(dir < 3);
    float factor = -(kenergy? sqrt_point_nine : 1);
    switch(dir)
    {
        case 0:
            _px += -_vx * dt;
            _vx *= factor;
            break;
        case 1:
            _py += -_vy * dt;
            _vy *= factor;
            break;
        case 2:
            _pz += -_vz * dt;
            _vz *= factor;
            break;
    }
}