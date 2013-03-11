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
    DestroyVBO();
}

bool SceneParticleBase::post_shader_compile()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    return true;
}

bool SceneParticleBase::create_shaders(const char* vsh, const char* fsh) 
{
    bool ret = SceneBase::create_shaders(vsh, fsh);
    return ret;
}

void SceneParticleBase::create_geom()
{
    CreateVBO();
}


bool SceneParticleBase::CreateVBO(void)
{

    GLenum ErrorCheckValue = glGetError();

    // Generate the buffers
    glGenBuffers(1, &startTime); // Start time buffers
    glGenBuffers(1, &initVel);   // Initial velocity buffer (never changes, only need one)
    glGenBuffers(1, &startPos);
    
    // Allocate space for all buffers
    int size = nParticles * 2 * sizeof(float);
    glBindBuffer(GL_ARRAY_BUFFER, initVel);
    glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, startTime);
    glBufferData(GL_ARRAY_BUFFER, nParticles * sizeof(float), NULL, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, startPos);
    glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_STATIC_DRAW);

    // Generate Velocities
    float x, y, velocity, theta, phi;
    GLfloat *data = new GLfloat[nParticles * 2];
    for (GLuint i = 0; i < nParticles; ++i)
    {
        // Pick the direction of the velocity
        theta = lerp(0.0f, get_spread(), randFloat());
        phi = lerp(0.0f, (float)TWOPI, randFloat());
        x = sinf(theta) * cosf(phi);
        y = cosf(theta);

        // Scale to set the magnitude of the velocity
        velocity = lerp(1.25f, 1.5f, randFloat());
        data[2 * i + 0] = x * velocity;
        data[2 * i + 1] = y * velocity;
    }

    glBindBuffer(GL_ARRAY_BUFFER, initVel);
    glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
    
    for (GLuint i = 0; i < nParticles; ++i)
    {
        data[2 * i + 0] = 0.0f;
        data[2 * i + 1] = -1.0f;
    }
    
    glBindBuffer(GL_ARRAY_BUFFER, startPos);
    glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
    
    // Fill the first start time buffer
    delete [] data;
    data = new GLfloat[nParticles];
    float time = 0.0f;
    float rate = 0.00075f;
    for(GLuint i = 0; i < nParticles; i++ ) {
        data[i] = time;
        time += rate;
    }
    glBindBuffer(GL_ARRAY_BUFFER, startTime);
    glBufferSubData(GL_ARRAY_BUFFER, 0, nParticles * sizeof(float), data);

    glBindBuffer(GL_ARRAY_BUFFER,0);
    delete [] data;
    
    // Create vertex arrays for each set of buffers
    glGenVertexArrays(1, &particles);
    glBindVertexArray(particles);
    
    glBindBuffer(GL_ARRAY_BUFFER, initVel);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, startTime);
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(1);
    
    glBindBuffer(GL_ARRAY_BUFFER, startPos);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
    
    ErrorCheckValue = glGetError();
    if (ErrorCheckValue != GL_NO_ERROR)
        return false;

    return true;
}

bool SceneParticleBase::DestroyVBO(void)
{
	GLenum ErrorCheckValue = glGetError();

        glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);
	
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // Velocity etc
        glDeleteBuffers(1, &initVel);
        glDeleteBuffers(1, &startTime);

        glBindVertexArray(0);
        glDeleteVertexArrays(nParticles, &particles);

        ErrorCheckValue = glGetError();
        if (ErrorCheckValue != GL_NO_ERROR)
            return false;
        return true;
}

void SceneParticleBase::on_pointer_moved(Vector2f location)
{
    GLfloat *data = new GLfloat[nParticles * 2];
    GLuint start_index = total_time/0.00075f;
    for (GLuint i = start_index; i < nParticles; ++i)
    {
        data[2 * i + 0] = location.X;
        data[2 * i + 1] = location.Y;
    }
    
    glBindBuffer(GL_ARRAY_BUFFER, startPos);
    glBufferSubData(GL_ARRAY_BUFFER, 0, nParticles * 2 * sizeof(float), data);
    delete [] data;
}

void SceneParticleBase::render(const float dt)
{      
    SceneBase::render(dt);

    glUniform1f(gTimeLocation, total_time);
    
    glBindVertexArray(particles);
    glDrawArrays(GL_POINTS, 0, nParticles);
    
    /* disable program */
    glUseProgram(0);
}