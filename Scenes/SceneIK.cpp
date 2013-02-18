/* 
 * File:   SceneIK.cpp
 * Author: sandy
 * 
 * Created on February 18, 2013, 7:33 AM
 */
#include <GL/glew.h>

#include "SceneIK.hpp"
#include "../IKLine.hpp"
#include "../math3d.hpp"

SceneIK::SceneIK()
{
}

SceneIK::SceneIK(const SceneIK& orig) 
{
}

SceneIK::~SceneIK() 
{
    //delete lines; lines = NULL;
}

/* Shaders */
bool SceneIK::create_shaders (const char* vsh)
{
    if (!SceneBase::create_shaders(VERTEX_SHADER))
        return false;
    gOrient = glGetUniformLocation(SHPROG, "o");
    if (gOrient == 0xFFFFFFFF)
        return false;
    
    gPos = glGetUniformLocation(SHPROG, "t");
    if (gPos == 0xFFFFFFFF)
        return false;

    return true;
}

/* Vertex array objects */
void SceneIK::create_vao()
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
    lines = new IKLine[3];// {IKLine(), IKLine(), IKLine(),};
    for (int i = 1; i < 3; ++i)
        lines[i].set_parent(&lines[i-1]);
}

void SceneIK::render()
{   
    SceneBase::render();
    
    for (int i = 0; i < 3; ++i)
    {
        /* Set current rendering shader */
        glUseProgram(SHPROG);
        
        /* Uniform update */
        glUniform1f(gOrient, lines[i].cum_orient());

        Vector2f base_pos = lines[i].cum_base();
        glUniform2fv(gPos, 1, (GLfloat*) &base_pos);

        // activate and specify pointer to vertex array
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(2, GL_FLOAT, 0, lines[i].vertices);

        /* Draw indices 0-1 which are 2 elements */
        glDrawRangeElements(GL_LINES, 0, 1, 2, GL_UNSIGNED_BYTE, 
                                               lines[i].indices);

        // deactivate vertex arrays after drawing
        glDisableClientState(GL_VERTEX_ARRAY);
        
        /* disable program */
	glUseProgram(0);
    }
}