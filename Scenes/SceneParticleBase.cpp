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
{
}

SceneParticleBase::SceneParticleBase(const SceneParticleBase& orig) 
{
}

SceneParticleBase::~SceneParticleBase() 
{
    if(startTimes)
        delete(startTimes); 
    startTimes = NULL;
    
    if(velocities)
        delete(velocities); 
    velocities = NULL;
    
    /*for (int i = 0; i < 2; ++i)
    {
        if(particleArray[i])
            delete(particleArray[i]); 
        particleArray[i] = NULL;
    }*/
    DestroyVBO();
}

bool SceneParticleBase::post_shader_compile()
{
    const char * outputNames[] = {"Position", "Velocity", "StartTime"};
    glTransformFeedbackVaryings(SHPROG, 3, outputNames, GL_SEPARATE_ATTRIBS);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    return true;
}

bool SceneParticleBase::create_shaders(const char* vsh) 
{
    bool ret = SceneBase::create_shaders(VERTEX_SHADER);
    gH = glGetUniformLocation(SHPROG, "H");
    if (gH == 0xFFFFFFFF)
        ret = false;
    updateSub = glGetSubroutineIndex(SHPROG, GL_VERTEX_SHADER, "update");
    renderSub = glGetSubroutineIndex(SHPROG, GL_VERTEX_SHADER, "render");
    return ret;
}

void SceneParticleBase::create_feedback_buffers()
{
    // Setup the feedback objects
    glGenTransformFeedbacks(2, feedback);
    
    // Transform feedback
    for (int i = 0; i > 2; ++i)
    {
        glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, feedback[i]);
        glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, posBuf[i]);
        glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 1, velBuf[i]);
        glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 2, startTime[i]);
    }
    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);
}

void SceneParticleBase::create_geom()
{
    //particleArray[0] = new GLfloat[3 * nParticles] {0.0f};
    //particleArray[1] = new GLfloat[3 * nParticles] {0.0f};
    //indices = new GLubyte[nParticles] {0};
    //for (int i = 0; i < nParticles; ++i)
    //    indices[i] = 3 * i;
    CreateVBO();
    create_feedback_buffers();
}


bool SceneParticleBase::CreateVBO(void)
{

    GLenum ErrorCheckValue = glGetError();

    // Generate the buffers
    glGenBuffers(2, posBuf);    // position buffers
    glGenBuffers(2, velBuf);    // velocity buffers
    glGenBuffers(2, startTime); // Start time buffers
    glGenBuffers(1, &initVel);  // Initial velocity buffer (never changes, only need one)

    // Allocate space for all buffers
    int size = nParticles * 3 * sizeof(float);
    glBindBuffer(GL_ARRAY_BUFFER, posBuf[0]);
    glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_DYNAMIC_COPY);
    glBindBuffer(GL_ARRAY_BUFFER, posBuf[1]);
    glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_DYNAMIC_COPY);
    glBindBuffer(GL_ARRAY_BUFFER, velBuf[0]);
    glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_DYNAMIC_COPY);
    glBindBuffer(GL_ARRAY_BUFFER, velBuf[1]);
    glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_DYNAMIC_COPY);
    glBindBuffer(GL_ARRAY_BUFFER, startTime[0]);
    glBufferData(GL_ARRAY_BUFFER, nParticles * sizeof(float), NULL, GL_DYNAMIC_COPY);
    glBindBuffer(GL_ARRAY_BUFFER, startTime[1]);
    glBufferData(GL_ARRAY_BUFFER, nParticles * sizeof(float), NULL, GL_DYNAMIC_COPY);
    glBindBuffer(GL_ARRAY_BUFFER, initVel);
    glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_STATIC_DRAW);
    
    // Fill the first position buffer with zeroes
    GLfloat *data = new GLfloat[nParticles * 3];
    for( int i = 0; i < nParticles * 3; i++ ) data[i] = 0.0f;
    glBindBuffer(GL_ARRAY_BUFFER, posBuf[0]);
    glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);

    // Generate Velocities
    float x, y, z, velocity, theta, phi;
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
    glBindBuffer(GL_ARRAY_BUFFER,velBuf[0]);
    glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
    glBindBuffer(GL_ARRAY_BUFFER,initVel);
    glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
    
    // Fill the first start time buffer
    delete [] data;
    data = new GLfloat[nParticles];
    float time = 0.0f;
    float rate = 0.001f;
    for( int i = 0; i < nParticles; i++ ) {
        data[i] = time;
        time += rate;
    }
    glBindBuffer(GL_ARRAY_BUFFER,startTime[0]);
    glBufferSubData(GL_ARRAY_BUFFER, 0, nParticles * sizeof(float), data);

    glBindBuffer(GL_ARRAY_BUFFER,0);
    delete [] data;
    
    // Create vertex arrays for each set of buffers
    glGenVertexArrays(2, particleArray);

    // Set up particle array 0
    glBindVertexArray(particleArray[0]);
    glBindBuffer(GL_ARRAY_BUFFER, posBuf[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, velBuf[0]);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, startTime[0]);
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, initVel);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(3);

    // Set up particle array 1
    glBindVertexArray(particleArray[1]);
    glBindBuffer(GL_ARRAY_BUFFER, posBuf[1]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, velBuf[1]);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, startTime[1]);
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, initVel);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(3);

    glBindVertexArray(0);
    
    ErrorCheckValue = glGetError();
    if (ErrorCheckValue != GL_NO_ERROR)
        return false;

    return true;
}

bool SceneParticleBase::DestroyVBO(void)
{
	GLenum ErrorCheckValue = glGetError();

	glDisableVertexAttribArray(3);
        glDisableVertexAttribArray(2);
        glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);
	
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // Velocity etc
        glDeleteBuffers(2, posBuf);
        glDeleteBuffers(2, velBuf);
        glDeleteBuffers(2, startTime);

        glBindVertexArray(0);
        glDeleteVertexArrays(nParticles, particleArray);

        ErrorCheckValue = glGetError();
        if (ErrorCheckValue != GL_NO_ERROR)
            return false;
        return true;
}

void SceneParticleBase::render(const float dt)
{      
    GLenum ErrorCheckValue;
    SceneBase::render(dt);
     /* Set current rendering shader */
    glUseProgram(SHPROG);   

    /********* Update pass *********/
    glUniformSubroutinesuiv(GL_VERTEX_SHADER, 1, &updateSub);
    glUniform1f(gH, dt);
    
    // Disable rendering
    glEnable(GL_RASTERIZER_DISCARD);
    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, feedback[drawbuff]);
    
    glBeginTransformFeedback(GL_POINTS);
      glBindVertexArray(particleArray[1-drawbuff]);
      glDrawArrays(GL_POINTS, 0, nParticles);
    glEndTransformFeedback();
    
    ErrorCheckValue = glGetError();
    if (ErrorCheckValue != GL_NO_ERROR)
        std::cout << "error detected in update" << std::endl;
    
    /********* Render pass *********/
    
    // Re-enable rendering
    glDisable(GL_RASTERIZER_DISCARD);
    glUniformSubroutinesuiv(GL_VERTEX_SHADER, 1, &renderSub);
    
    /* Uniform update */ // TODO check if needed
    glUniformMatrix4fv(gWorldLocation, 1, GL_TRUE, World->m);
   
    // Unbind feedback object 
    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);
    
    // Draw
    glBindVertexArray(particleArray[drawbuff]);
    glDrawArrays(GL_POINTS, 0, nParticles);
    
    // Swap buffers
    drawbuff = 1 - drawbuff;
    
    /* disable program */
    glUseProgram(0);
    
    ErrorCheckValue = glGetError();
    if (ErrorCheckValue != GL_NO_ERROR)
        std::cout << "error detected in draw" << std::endl;
}