#include <GL/glew.h>
#include <GL/glu.h>

#include <gtkmm.h>
#include <gtkglmm.h>

#include <fstream>
#include <string>
#include <iostream>
#include <assert.h>

#include "../md5/Joint.hpp"
#include "../md5/Mesh.hpp"
#include "../md5/Weight.hpp"

#include "../math3d.hpp"

#include "SceneBase.hpp"
#include "SceneMD5.hpp"

SceneMD5::SceneMD5()
{
}

SceneMD5::~SceneMD5()
{
    DestroyVBO();
    delete [] joints;
    joints = NULL;
}

void SceneMD5::load_md5_mesh(const char* filename)
{
    std::ifstream input;
    input.open(filename);
    
    //std::cout << filename << std::endl;
    
    std::string keyword;
    
    while (input >> keyword) {
        // Allocate memory for the joints
        if (keyword == "numJoints") {
            input >> numJoints;
            joints = new Joint[numJoints];
        }
        // Allocate memory for the Meshes
        else if (keyword == "numMeshes") {
            input >> numMeshes;
            meshes = new Mesh[numMeshes];
        }
        // Read the joints
        else if( keyword == "joints" && input >> keyword) {
            assert(keyword == "{" );
            for(uint i = 0; i != numJoints; ++i)
                input >> joints[i];
        }
        // Read the mesh
        else if (keyword == "mesh" && input >> keyword) {
            assert(keyword == "{" );
            for(uint i = 0; i != numMeshes; ++i) 
                input >> meshes[i];
        }
    } // Read Loop
    input.close();
    
    DestroyVBO();
    create_geom();
}

bool SceneMD5::create_shaders(const char* vsh, const char* fsh) 
{
    
    bool ret = true;
    // Create the joint shader program
    ret &= create_shader(JOINTSHVERT, JOINTSHFRAG, JOINTSHPROG, 
                         JOINT_VERTEX_SHADER, JOINT_FRAGMENT_SHADER);
    return ret;
}

bool SceneMD5::create_shader (GLuint& shvert, GLuint& shfrag, GLuint& shprog,
                              const char* vertex_shader, 
                              const char* fragment_shader)
{
    char shader_error[1024];
    int error_length = 0;
    GLint res;

    /* Generate some IDs for our shader programs */
    shvert = glCreateShader(GL_VERTEX_SHADER);
    shfrag = glCreateShader(GL_FRAGMENT_SHADER);
    shprog = glCreateProgram();

    /* Assign shader source code to these IDs */
    glShaderSource(shvert, 1, &vertex_shader, NULL);
    glShaderSource(shfrag, 1, &fragment_shader, NULL);

    /* Compile the code */
    glCompileShader(shvert);
    glCompileShader(shfrag);

    /* Check if compilation was successful */
    glGetShaderiv(shvert, GL_COMPILE_STATUS, &res);
    if (res == GL_FALSE)
    {
        std::cerr << "Compilation of vertex shader failed" << std::endl;
        glGetShaderInfoLog(shvert, 1024, &error_length, shader_error);
        std::cerr << shader_error << std::endl;
        return false;
    }
    glGetShaderiv(shfrag, GL_COMPILE_STATUS, &res);
    if (res == GL_FALSE)
    {
        std::cerr << "Compilation of fragment shader failed" << std::endl;
        glGetShaderInfoLog(shfrag, 1024, &error_length, shader_error);
        std::cerr << shader_error << std::endl;
        return false;
    }

    /* Attach these shaders to the shader program */
    glAttachShader(shprog, shvert);
    glAttachShader(shprog, shfrag);

    /* Flag the shaders to be deleted when the shader program is deleted */
    glDeleteShader(shvert);
    glDeleteShader(shfrag);

    if (!post_shader_compile())
        return false;
    
    /* Link the shaders */
    glLinkProgram(shprog);
    glGetProgramiv(shprog, GL_LINK_STATUS, &res);
    if (res == GL_FALSE)
        std::cerr << "Failed to link shader program" << std::endl;

    glUseProgram(shprog);
    
    /* Get Uniforms */
    gWorldLocation = glGetUniformLocation(shprog, "gWorld");
    if (gWorldLocation == 0xFFFFFFFF)
        return false;
    gTimeLocation = glGetUniformLocation(shprog, "gTime");
    if (gTimeLocation == 0xFFFFFFFF)
        std::cout << "gTime is not used in this shader" << std::endl;
    
    return true;
}

void SceneMD5::create_light()
{
}

void SceneMD5::create_geom()
{
    if(!joints)
        return;
     CreateVBO();
}

bool SceneMD5::CreateVBO(void)
{
    return CreateJointVBO();
}


bool SceneMD5::CreateJointVBO(void)
{
    int sizeCoord = numJoints * 3;
    
    GLenum ErrorCheckValue = glGetError();

    // Generate the joint location buffer
    glGenBuffers(1, &jointPtr);
    
    glBindBuffer(GL_ARRAY_BUFFER, jointPtr);
    glBufferData(GL_ARRAY_BUFFER, sizeCoord * sizeof(float), NULL, GL_STATIC_DRAW);

    GLfloat *data = new GLfloat[sizeCoord];
    for (GLuint i = 0; i < numJoints; ++i)
    {
        glm::vec3 p = joints[i].getPosition();
        data[3 * i + 0] = p.x;
        data[3 * i + 1] = p.y;
        data[3 * i + 2] = p.z;
    }
    
    glBindBuffer(GL_ARRAY_BUFFER, jointPtr);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeCoord * sizeof(float), data);
    delete [] data;
    
    // Generate the depth buffer
    glGenBuffers(1, &jointDepthPtr);
    
    glBindBuffer(GL_ARRAY_BUFFER, jointDepthPtr);
    glBufferData(GL_ARRAY_BUFFER, numJoints * sizeof(float), NULL, GL_STATIC_DRAW);

    data = new GLfloat[numJoints];
    for (GLuint i = 0; i < numJoints; ++i)
        data[i] = (float)(joints[i].getParentIndex() + 1) / numJoints;
    
    glBindBuffer(GL_ARRAY_BUFFER, jointDepthPtr);
    glBufferSubData(GL_ARRAY_BUFFER, 0, numJoints * sizeof(float), data);
    delete [] data;
    
    
    // ---
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    // Create vertex arrays for each set of buffers
    glGenVertexArrays(1, &jointsVAO);
    glBindVertexArray(jointsVAO);
    
    // Position buffer
    glBindBuffer(GL_ARRAY_BUFFER, jointPtr);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);
    
    // Depth buffer
    glBindBuffer(GL_ARRAY_BUFFER, jointDepthPtr);
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
    
    ErrorCheckValue = glGetError();
    if (ErrorCheckValue != GL_NO_ERROR)
        return false;

    return true;
}

bool SceneMD5::DestroyVBO(void)
{
	GLenum ErrorCheckValue = glGetError();

	glDisableVertexAttribArray(0);
	
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glDeleteBuffers(0, &jointPtr);

        glBindVertexArray(0);
        glDeleteVertexArrays(numJoints, &jointsVAO);

        ErrorCheckValue = glGetError();
        if (ErrorCheckValue != GL_NO_ERROR)
            return false;
        return true;
}

void SceneMD5::set_perspective()
{
}

void SceneMD5::render(const float dt)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    renderJoints(dt);
    
    // Disable program
    glUseProgram(0);
}

void SceneMD5::renderJoints(const float dt)
{
    
    /* Set current rendering shader */
    glUseProgram(JOINTSHPROG);
    
    /* Uniform update */
    glUniformMatrix4fv(gWorldLocation, 1, GL_TRUE, World->m);
    glUniform1f(gTimeLocation, total_time);
    
    glBindVertexArray(jointsVAO);
    glDrawArrays(GL_POINTS, 0, numJoints);
    glDrawArrays(GL_LINE_STRIP, 0, numJoints);
    glDrawArrays(GL_LINES_ADJACENCY, 0, numJoints);
}

void SceneMD5::update(const float dt)
{
    SceneBase::update(dt);
}