/* 
 * File:   SceneParticleBase.cpp
 * Author: sandy
 * 
 * Created on March 9, 2013, 3:28 PM
 */
// TODO TEMP
#include <iostream>
#include <GL/glew.h>
#include "../math3d.hpp"

#include "SceneParticleBase.hpp"

SceneParticleBase::SceneParticleBase() 
    : origin(new Vector2f(0.0f, 0.0f))
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
    bool ret = SceneBase::create_shaders(VERTEX_SHADER, FRAGMENT_SHADER);
    gOrigin = glGetUniformLocation(SHPROG, "origin");
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

    // Allocate space for all buffers
    int size = nParticles * 3 * sizeof(float);
    glBindBuffer(GL_ARRAY_BUFFER, initVel);
    glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, startTime);
    glBufferData(GL_ARRAY_BUFFER, nParticles * sizeof(float), NULL, GL_STATIC_DRAW);

    // Generate Velocities
    float x, y, z, velocity, theta, phi;
    GLfloat *data = new GLfloat[nParticles * 3];
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
        data[3 * i + 0] = x * velocity;
        data[3 * i + 1] = y * velocity;
        data[3 * i + 2] = z * velocity;
    }

    glBindBuffer(GL_ARRAY_BUFFER,initVel);
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, startTime);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(1);

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
    delete(origin);
    origin = new Vector2f(location);
}

void SceneParticleBase::render(const float dt)
{      
    GLenum ErrorCheckValue;
    SceneBase::render(dt);

    glUniform1f(gTimeLocation, total_time);
    glUniform2fv(gOrigin, 1, (GLfloat*)origin);
    
    glBindVertexArray(particles);
    glDrawArrays(GL_POINTS,0,nParticles);
    
    /* disable program */
    glUseProgram(0);
    
    ErrorCheckValue = glGetError();
    if (ErrorCheckValue != GL_NO_ERROR)
        std::cout << "error detected in draw" << std::endl;
}