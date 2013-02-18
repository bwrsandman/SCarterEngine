/* 
 * File:   SceneMorph.cpp
 * Author: sandy
 * 
 * Created on February 17, 2013, 11:46 PM
 */

#include <iostream>
#include <GL/glew.h>
#include <GL/glu.h>

#include "SceneBase.hpp"
#include "SceneMorph.hpp"
#include "../math3d.hpp"

SceneMorph::SceneMorph() {
}

SceneMorph::SceneMorph(const SceneMorph& orig) {
}

SceneMorph::~SceneMorph() {
}

/* Shaders */
bool SceneMorph::create_shaders (const char* vsh)
{
    if (!SceneBase::create_shaders(VERTEX_SHADER))
        return false;
    gAlpha = glGetUniformLocation(SHPROG, "alpha");
    if (gAlpha == 0xFFFFFFFF)
        return false;

    return true;
}

/* Vertex array objects */
void SceneMorph::create_vao (void)
{
    /************************
     *
     *      x-------------x
     *     /|            /|
     *    / |           / |
     *   /  |          /  |
     *  x-------------x   |
     *  |   |    |_   |   |
     *  |   |    /    |   |
     *  |   x---------|---x
     *  |  /          |  /
     *  | /           | /
     *  |/            |/
     *  x-------------x
     *
     ************************/

    vertices = new GLfloat[12 * subs * subs * 2];// = { 0 };
    indices = new GLubyte[ 6 * subs * subs * 2];

    // Back
    GLfloat *v = vertices;
    GLubyte *in = indices;

    for (int j=0; j < subs; ++j)
        for (int i=0; i < subs; ++i)
        {
            // v0+4i
            v[j * subs * 12 + i * 12 +  0] = -si + 2.0f / subs * si * i;        // X
            v[j * subs * 12 + i * 12 +  1] = -si + 2.0f / subs * si * j;        // Y
            v[j * subs * 12 + i * 12 +  2] = -si + 0.0f * si;                   // Z
            // v1+4i
            v[j * subs * 12 + i * 12 +  3] = -si + 2.0f / subs * si * i;        // X
            v[j * subs * 12 + i * 12 +  4] = -si + 2.0f / subs * si * (j + 1);  // Y
            v[j * subs * 12 + i * 12 +  5] = -si + 0.0f * si;                   // Z
            // v2+4i
            v[j * subs * 12 + i * 12 +  6] = -si + 2.0f / subs * si * (i + 1);  // X
            v[j * subs * 12 + i * 12 +  7] = -si + 2.0f / subs * si * j;        // Y
            v[j * subs * 12 + i * 12 +  8] = -si + 0.0f * si;                   // Z
            // v3+4i
            v[j * subs * 12 + i * 12 +  9] = -si + 2.0f / subs * si * (i + 1);  // X
            v[j * subs * 12 + i * 12 + 10] = -si + 2.0f / subs * si * (j + 1);  // Y
            v[j * subs * 12 + i * 12 + 11] = -si + 0.0f * si;                   // Z
        }

    for (int j=0; j < subs; ++j)
        for (int i=0; i < subs; ++i)    
        {
            in[j * subs * 6 + i * 6 + 0] = j * subs * subs + i * subs + 0;
            in[j * subs * 6 + i * 6 + 1] = j * subs * subs + i * subs + 1;
            in[j * subs * 6 + i * 6 + 2] = j * subs * subs + i * subs + 2;

            in[j * subs * 6 + i * 6 + 3] = j * subs * subs + i * subs + 2;
            in[j * subs * 6 + i * 6 + 4] = j * subs * subs + i * subs + 3;
            in[j * subs * 6 + i * 6 + 5] = j * subs * subs + i * subs + 1;
        }

    // Front
    v  += subs * subs * 12;
    in += subs * subs * 6;

    for (int j=0; j < subs; ++j)
        for (int i=0; i < subs; ++i)
        {
            // v0+4i
            v[j * subs * 12 + i * 12 +  0] = -si + 2.0f / subs * si * i;        // X
            v[j * subs * 12 + i * 12 +  1] = -si + 2.0f / subs * si * j;        // Y
            v[j * subs * 12 + i * 12 +  2] = si;                                // Z
            // v1+4i
            v[j * subs * 12 + i * 12 +  3] = -si + 2.0f / subs * si * i;        // X
            v[j * subs * 12 + i * 12 +  4] = -si + 2.0f / subs * si * (j + 1);  // Y
            v[j * subs * 12 + i * 12 +  5] = si;                                // Z
            // v2+4i
            v[j * subs * 12 + i * 12 +  6] = -si + 2.0f / subs * si * (i + 1);  // X
            v[j * subs * 12 + i * 12 +  7] = -si + 2.0f / subs * si * j;        // Y
            v[j * subs * 12 + i * 12 +  8] = si;                                // Z
            // v3+4i
            v[j * subs * 12 + i * 12 +  9] = -si + 2.0f / subs * si * (i + 1);  // X
            v[j * subs * 12 + i * 12 + 10] = -si + 2.0f / subs * si * (j + 1);  // Y
            v[j * subs * 12 + i * 12 + 11] = si;                                // Z
        }

    for (int j=0; j < subs; ++j)
        for (int i=0; i < subs; ++i)    
        {
            in[j * subs * 6 + i * 6 + 0] = j * subs * subs + i * subs + 0;
            in[j * subs * 6 + i * 6 + 1] = j * subs * subs + i * subs + 1;
            in[j * subs * 6 + i * 6 + 2] = j * subs * subs + i * subs + 2;

            in[j * subs * 6 + i * 6 + 3] = j * subs * subs + i * subs + 2;
            in[j * subs * 6 + i * 6 + 4] = j * subs * subs + i * subs + 3;
            in[j * subs * 6 + i * 6 + 5] = j * subs * subs + i * subs + 1;
        }

    // Left
    v  += subs * subs * 12;
    in += subs * subs * 6;
}

/* Draw Scene */
void SceneMorph::render (GLenum draw_type)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /* Uniform update */
    glUniformMatrix4fv(gWorldLocation, 1, GL_TRUE, World->m);
    glUniform1f(gAlpha, alpha);

    // activate and specify pointer to vertex array
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vertices);

    // draw first half, range is 6 - 0 + 1 = 7 vertices used
    glDrawRangeElements(GL_TRIANGLES, 0, 4 * subs * subs - 1, 6 * subs * subs, GL_UNSIGNED_BYTE, indices);

    // draw second half, range is 7 - 1 + 1 = 7 vertices used
    //glDrawRangeElements(draw_type, 1, 7, 18, GL_UNSIGNED_BYTE, indices+18);

    // deactivate vertex arrays after drawing
    glDisableClientState(GL_VERTEX_ARRAY);

    /*** OpenGL END ***/
}