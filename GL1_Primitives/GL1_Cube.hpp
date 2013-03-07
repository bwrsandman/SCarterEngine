/* 
 * File:   GL1_Cube.hpp
 * Author: sandy
 *
 * Created on March 7, 2013, 4:18 PM
 */

#ifndef GL1_CUBE_HPP
#define	GL1_CUBE_HPP

#include "GL1_Sphere.hpp"

/* Cube ********************************************************************
 *
 *    v6----- v5
 *   /|      /|
 *  v1------v0|
 *  | |     | |
 *  | |v7---|-|v4
 *  |/      |/
 *  v2------v3
 *
 **************************************************************************/

const float vertices[] =
{
    // v0-v1-v2 (front)
      1.0f,  1.0f,  1.0f,  - 1.0f,  1.0f,  1.0f,  - 1.0f,- 1.0f,  1.0f,
    // v2-v3-v0
    - 1.0f,- 1.0f,  1.0f,    1.0f,- 1.0f,  1.0f,    1.0f,  1.0f,  1.0f,
    // v0-v3-v4 (right)
      1.0f,  1.0f,  1.0f,    1.0f,- 1.0f,  1.0f,    1.0f,- 1.0f,- 1.0f,
    // v4-v5-v0
      1.0f,- 1.0f,- 1.0f,    1.0f,  1.0f,- 1.0f,    1.0f,  1.0f,  1.0f,
    // v0-v5-v6 (top)
      1.0f,  1.0f,  1.0f,    1.0f,  1.0f,- 1.0f,  - 1.0f,  1.0f,- 1.0f,
    // v6-v1-v0
    - 1.0f,  1.0f,- 1.0f,  - 1.0f,  1.0f,  1.0f,    1.0f,  1.0f,  1.0f,
    // v1-v6-v7 (left)
    - 1.0f,  1.0f,  1.0f,  - 1.0f,  1.0f,- 1.0f,  - 1.0f,- 1.0f,- 1.0f,
    // v7-v2-v1
    - 1.0f,- 1.0f,- 1.0f,  - 1.0f,- 1.0f,  1.0f,  - 1.0f,  1.0f,  1.0f,
    // v7-v4-v3 (bottom)
    - 1.0f,- 1.0f,- 1.0f,    1.0f,- 1.0f,- 1.0f,    1.0f,- 1.0f,  1.0f,
    // v3-v2-v7
      1.0f,- 1.0f,  1.0f,  - 1.0f,- 1.0f,  1.0f,  - 1.0f,- 1.0f,- 1.0f,
    // v4-v7-v6 (back)
      1.0f,- 1.0f,- 1.0f,  - 1.0f,- 1.0f,- 1.0f,  - 1.0f,  1.0f,- 1.0f,
    // v6-v5-v4
    - 1.0f,  1.0f,- 1.0f,    1.0f,  1.0f,- 1.0f,    1.0f,- 1.0f,- 1.0f
 };

const float normals[]  =
{
    // v0-v1-v2 (front)
     0.0f, 0.0f, 1.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, 1.0f,
    // v2-v3-v0
     0.0f, 0.0f, 1.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, 1.0f,
    // v0-v3-v4 (right)
     1.0f, 0.0f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 0.0f, 0.0f,
    // v4-v5-v0
     1.0f, 0.0f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 0.0f, 0.0f,
    // v0-v5-v6 (top)
     0.0f, 1.0f, 0.0f,   0.0f, 1.0f, 0.0f,   0.0f, 1.0f, 0.0f,
    // v6-v1-v0
     0.0f, 1.0f, 0.0f,   0.0f, 1.0f, 0.0f,   0.0f, 1.0f, 0.0f,
    // v1-v6-v7 (left)
    -1.0f, 0.0f, 0.0f,  -1.0f, 0.0f, 0.0f,  -1.0f, 0.0f, 0.0f,
    // v7-v2-v1
    -1.0f, 0.0f, 0.0f,  -1.0f, 0.0f, 0.0f,  -1.0f, 0.0f, 0.0f,
    // v7-v4-v3 (bottom)
     0.0f,-1.0f, 0.0f,   0.0f,-1.0f, 0.0f,   0.0f,-1.0f, 0.0f,
    // v3-v2-v7
     0.0f,-1.0f, 0.0f,   0.0f,-1.0f, 0.0f,   0.0f,-1.0f, 0.0f,
    // v4-v7-v6 (back)
     0.0f, 0.0f,-1.0f,   0.0f, 0.0f,-1.0f,   0.0f, 0.0f,-1.0f,
    // v6-v5-v4
     0.0f, 0.0f,-1.0f,   0.0f, 0.0f,-1.0f,   0.0f, 0.0f,-1.0f
};

const float colours[] =
{
    //v0-v1-v2 (front)
    1.0f, 1.0f, 1.0f,   1.0f, 1.0f, 0.0f,   1.0f, 0.0f, 0.0f,
    // v2-v3-v0
    1.0f, 0.0f, 0.0f,   1.0f, 0.0f, 1.0f,   1.0f, 1.0f, 1.0f,
    // v0-v3-v4 (right)
    1.0f, 1.0f, 1.0f,   1.0f, 0.0f, 1.0f,   0.0f, 0.0f, 1.0f,
    // v4-v5-v0
    0.0f, 0.0f, 1.0f,   0.0f, 1.0f, 1.0f,   1.0f, 1.0f, 1.0f,
    // v0-v5-v6 (top)
    1.0f, 1.0f, 1.0f,   0.0f, 1.0f, 1.0f,   0.0f, 1.0f, 0.0f,
    // v6-v1-v0
    0.0f, 1.0f, 0.0f,   1.0f, 1.0f, 0.0f,   1.0f, 1.0f, 1.0f,
    // v1-v6-v7 (left)
    1.0f, 1.0f, 0.0f,   0.0f, 1.0f, 0.0f,   0.0f, 0.0f, 0.0f,
    // v7-v2-v1
    0.0f, 0.0f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, 0.0f,
    // v7-v4-v3 (bottom)
    0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 1.0f,   1.0f, 0.0f, 1.0f,
    // v3-v2-v7
    1.0f, 0.0f, 1.0f,   1.0f, 0.0f, 0.0f,   0.0f, 0.0f, 0.0f,
    // v4-v7-v6 (back)
    0.0f, 0.0f, 1.0f,   0.0f, 0.0f, 0.0f,   0.0f, 1.0f, 0.0f,
    // v6-v5-v4
    0.0f, 1.0f, 0.0f,   0.0f, 1.0f, 1.0f,   0.0f, 0.0f, 1.0f
};

void drawCube (float);

class GL1_Cube {
public:
    GL1_Cube(float);
    GL1_Cube(const GL1_Cube&);
    virtual ~GL1_Cube();
    
    void apply_gravity(GL1_Sphere&) const;
    void apply_drag(GL1_Sphere&) const;
    void draw(void);
    
    inline float right(void) const  { return _px + _side / 2.0f; }
    inline float left(void) const   { return _px - _side / 2.0f; }
    inline float top(void) const    { return _py + _side / 2.0f; }
    inline float bottom(void) const { return _py - _side / 2.0f; }
    inline float front(void) const  { return _py + _side / 2.0f; }
    inline float back(void) const   { return _py - _side / 2.0f; }
        
private:
    float _side;
    float _px =    0.0f;
    float _py =    0.0f;
    float _pz =    0.0f;
    float _gx =    0.0f;
    float _gy =   -0.0000098f;
    float _gz =    0.0f;
    float _drag =  0.1f;
    
    
};

#endif	/* GL1_CUBE_HPP */

