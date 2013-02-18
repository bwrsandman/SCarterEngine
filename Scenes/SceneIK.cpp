/* 
 * File:   SceneIK.cpp
 * Author: sandy
 * 
 * Created on February 18, 2013, 7:33 AM
 */
#include <GL/glew.h>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

#include "SceneIK.hpp"
#include "../IKArm.hpp"
#include "../IKLine.hpp"
#include "../math3d.hpp"

SceneIK::SceneIK()
{
    /* initialize random seed: */
    srand (time(NULL));
    
    target_pos = random_spot();
}

SceneIK::SceneIK(const SceneIK& orig) 
{
}

SceneIK::~SceneIK() 
{
    delete target_pos; target_pos= NULL; 
    delete arm; arm = NULL;
}

Vector2f *SceneIK::random_spot()
{
    return new Vector2f(2.0f * (float)rand()/(float)RAND_MAX - 1.0f,
                        2.0f * (float)rand()/(float)RAND_MAX - 1.0f);
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
    arm = new IKArm();
}

void SceneIK::render()
{   
    // TODO use proper update(t) function
    arm->move_to(*target_pos, 0.017f);
    timer += 0.017f;
    
    if (timer > 5.0f)
    {
        timer -= 5.0f;
        delete target_pos;
        target_pos = random_spot();
    }
    
    
    SceneBase::render();
    
    for (int i = 0; i < 3; ++i)
    {
        /* Set current rendering shader */
        glUseProgram(SHPROG);
        
        /* Uniform update */
        glUniform1f(gOrient, arm->lines[i].cum_orient());

        Vector2f base_pos = arm->lines[i].cum_base();
        glUniform2fv(gPos, 1, (GLfloat*) &base_pos);

        // activate and specify pointer to vertex array
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(2, GL_FLOAT, 0, arm->lines[i].vertices);

        /* Draw indices 0-1 which are 2 elements */
        glDrawRangeElements(GL_LINES,  0, 1, 2, GL_UNSIGNED_BYTE, 
                                                arm->lines[i].indices);
        glDrawRangeElements(GL_POINTS, 0, 1, 2, GL_UNSIGNED_BYTE, 
                                                arm->lines[i].indices);

        // deactivate vertex arrays after drawing
        glDisableClientState(GL_VERTEX_ARRAY);
        
        /* disable program */
	glUseProgram(0);
    }
    
    /* Set current rendering shader */
    glUseProgram(SHPROG);

    /* Uniform update */
    glUniform1f(gOrient, 0.0f);
    
    glUniform2fv(gPos, 1, (GLfloat*) target_pos);

    // activate and specify pointer to vertex array
    glEnableClientState(GL_VERTEX_ARRAY);
    Vector2f point_vertex[] = {Vector2f()};
    glVertexPointer(2, GL_FLOAT, 0, &point_vertex);

    /* Draw index 0 which is 1 elements */
    GLubyte point_index = 0;
    glDrawRangeElements(GL_POINTS, 0, 0, 1, GL_UNSIGNED_BYTE, 
                                            &point_index);

    // deactivate vertex arrays after drawing
    glDisableClientState(GL_VERTEX_ARRAY);

    /* disable program */
    glUseProgram(0);
}