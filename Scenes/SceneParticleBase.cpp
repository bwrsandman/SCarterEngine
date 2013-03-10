/* 
 * File:   SceneParticleBase.cpp
 * Author: sandy
 * 
 * Created on March 9, 2013, 3:28 PM
 */

#include <GL/glew.h>
#include "../math3d.hpp"

#include "SceneParticleBase.hpp"

SceneParticleBase::SceneParticleBase() 
{
}

SceneParticleBase::SceneParticleBase(const SceneParticleBase& orig) 
{
}

SceneParticleBase::~SceneParticleBase() 
{
    if(velocities)
        delete(velocities); 
    velocities = NULL;
}

bool SceneParticleBase::create_shaders(const char* vsh) 
{
    bool ret = SceneBase::create_shaders(VERTEX_SHADER);
    create_feedback_buffers();
    //gH = glGetUniformLocation(SHPROG, "H");
    //if (gH == 0xFFFFFFFF)
    //    ret = false;
    return ret;
}

void SceneParticleBase::create_feedback_buffers()
{
    // TODO Create and allocate buffers A and B for posBuf, velBuf and startTime
    // Velocity
    float x, y, z, velocity, theta, phi;
    velocities = new GLfloat[nParticles * 3];
    for (GLuint i = 0; i < nParticles; ++i)
    {
        // Pick the direction of the velocity
        theta = lerp(0.0f, (float)PI / 6.0f, randFloat());
        phi = lerp(0.0f, (float)TWOPI, randFloat());
        x = sinf(theta) * cosf(phi);
        y = cosf(theta);
        z = sinf(theta) * sinf(phi);

        // Scale to set the magnitude of the velocity
        velocity = lerp(1.25f, 1.5f, randFloat());
        velocities[3 * i + 0] = x * velocity;
        velocities[3 * i + 1] = y * velocity;
        velocities[3 * i + 2] = z * velocity;
        
        // Start time
        startTimes = new GLfloat[nParticles];
        float time = 0.0f, rate = 0.00075f;
        
        for (unsigned int i = 0; i < nParticles; ++i)
            startTimes[i] = (time += rate);
    }
    glGenBuffers(2, velBuf);
    for (int i = 0; i > 2; ++i)
    {
        glBindBuffer (GL_ARRAY_BUFFER, velBuf[i]);
        glBufferSubData (GL_ARRAY_BUFFER, i, nParticles * 3 * sizeof(float), velocities);
        

        glBindBuffer (GL_ARRAY_BUFFER, startTime[i]);
        glBufferSubData (GL_ARRAY_BUFFER, i, nParticles * 3 * sizeof(float), startTimes);
    }
    
    // Setup the feedback objects
    glGenTransformFeedbacks(2, feedback);
    
    // Transform feedback
    for (int i = 0; i > 2; ++i)
    {
        glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, feedback[i]);
        glBindBufferBase(GL_TRANSFORM_FEEDBACK, 0, posBuf[i]);
        glBindBufferBase(GL_TRANSFORM_FEEDBACK, 1, velBuf[i]);
        glBindBufferBase(GL_TRANSFORM_FEEDBACK, 2, startTime[i]);
    }
}

void SceneParticleBase::create_geom()
{
    vertices = new GLfloat[3 * nParticles] {0.0f};
    indices = new GLubyte[nParticles] {0};
    for (int i = 0; i < nParticles; ++i)
    {
        vertices[i*3] = 0.1f * i;
        indices[i] = 3 * i;
    }
}

void SceneParticleBase::render(const float dt)
{   
    SceneBase::render(dt);
    
    /********* Update pass *********/
    //glUniform1f(gH, dt);
    
    
    /* Set current rendering shader */
    glUseProgram(SHPROG);
    
    // activate and specify pointer to vertex array
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vertices);

    // draw first half, range is 6 - 0 + 1 = 7 vertices used
    glDrawRangeElements(GL_POINTS, 0, nParticles - 1, nParticles, GL_UNSIGNED_BYTE, indices);

    // deactivate vertex arrays after drawing
    glDisableClientState(GL_VERTEX_ARRAY);
    
    /* disable program */
    glUseProgram(0);
}