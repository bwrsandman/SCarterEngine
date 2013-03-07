/* 
 * File:   GL1_Cube.cpp
 * Author: sandy
 * 
 * Created on March 7, 2013, 4:18 PM
 */

#include <GL/gl.h>
#include "GL1_Cube.hpp"

GL1_Cube::GL1_Cube(float side)
    : _side(side)
{
}

GL1_Cube::GL1_Cube(const GL1_Cube& orig) {
}

GL1_Cube::~GL1_Cube() {
}

void drawCube (float size)
{
    const GLfloat offset = size / 2.0f;
    glBegin(GL_TRIANGLES);
    for(int i = 0; i < 30; ++i)
    {
        glNormal3f(normals[i*3], normals[i*3+1], normals[i*3+2]);
        glColor4f(colours[i*3], colours[i*3+1], colours[i*3+2], 0.2);
        glVertex3f(offset * vertices[i*3],
                   offset * vertices[i*3+1],
                   offset * vertices[i*3+2]);
    }
    glEnd();
}

void GL1_Cube::apply_gravity(GL1_Sphere &sphere) const
{
        sphere.fx(_gx * sphere.mass());
        sphere.fy(_gy * sphere.mass());
        sphere.fz(_gz * sphere.mass());
}

void GL1_Cube::apply_drag(GL1_Sphere &sphere) const
{
        float drag = -_drag * sphere.speed();
        sphere.fx(drag * sphere.vx());
        sphere.fy(drag * sphere.vy());
        sphere.fz(drag * sphere.vz());
}

void GL1_Cube::draw(void)
{
        drawCube(_side);
}