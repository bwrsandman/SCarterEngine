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
    bool ret = create_joint_shader();
    return ret;
}

bool SceneMD5::create_joint_shader ()
{
    char shader_error[1024];
    int error_length = 0;
    GLint res;

    /* Generate some IDs for our shader programs */
    JOINTSHVERT = glCreateShader(GL_VERTEX_SHADER);
    JOINTSHFRAG = glCreateShader(GL_FRAGMENT_SHADER);
    JOINTSHPROG = glCreateProgram();

    /* Assign shader source code to these IDs */
    glShaderSource(JOINTSHVERT, 1, &JOINT_VERTEX_SHADER, NULL);
    glShaderSource(JOINTSHFRAG, 1, &JOINT_FRAGMENT_SHADER, NULL);

    /* Compile the code */
    glCompileShader(JOINTSHVERT);
    glCompileShader(JOINTSHFRAG);

    /* Check if compilation was successful */
    glGetShaderiv(JOINTSHVERT, GL_COMPILE_STATUS, &res);
    if (res == GL_FALSE)
    {
        std::cerr << "Compilation of vertex shader failed" << std::endl;
        glGetShaderInfoLog(JOINTSHVERT, 1024, &error_length, shader_error);
        std::cerr << shader_error << std::endl;
        return false;
    }
    glGetShaderiv(JOINTSHFRAG, GL_COMPILE_STATUS, &res);
    if (res == GL_FALSE)
    {
        std::cerr << "Compilation of fragment shader failed" << std::endl;
        glGetShaderInfoLog(JOINTSHFRAG, 1024, &error_length, shader_error);
        std::cerr << shader_error << std::endl;
        return false;
    }

    /* Attach these shaders to the shader program */
    glAttachShader(JOINTSHPROG, JOINTSHVERT);
    glAttachShader(JOINTSHPROG, JOINTSHFRAG);

    /* Flag the shaders to be deleted when the shader program is deleted */
    glDeleteShader(JOINTSHVERT);
    glDeleteShader(JOINTSHFRAG);

    if (!post_shader_compile())
        return false;
    
    /* Link the shaders */
    glLinkProgram(JOINTSHPROG);
    glGetProgramiv(JOINTSHPROG, GL_LINK_STATUS, &res);
    if (res == GL_FALSE)
        std::cerr << "Failed to link shader program" << std::endl;

    glUseProgram(JOINTSHPROG);
    
    /* Get Uniforms */
    gWorldLocation = glGetUniformLocation(JOINTSHPROG, "gWorld");
    if (gWorldLocation == 0xFFFFFFFF)
        return false;
    gTimeLocation = glGetUniformLocation(JOINTSHPROG, "gTime");
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
    int size = sizeCoord * sizeof(float);
    
    GLenum ErrorCheckValue = glGetError();

    // Generate the buffers
    glGenBuffers(1, &jointPtr);
    
    glBindBuffer(GL_ARRAY_BUFFER, jointPtr);
    glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_STATIC_DRAW);

    GLfloat *data = new GLfloat[sizeCoord];
    for (GLuint i = 0; i < numJoints; ++i)
    {
        glm::vec3 p = joints[i].getPosition();
        data[3 * i + 0] = p.x;
        data[3 * i + 1] = p.y;
        data[3 * i + 2] = p.z;
    }
    
    glBindBuffer(GL_ARRAY_BUFFER, jointPtr);
    glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
    delete [] data;
    
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    // Create vertex arrays for each set of buffers
    glGenVertexArrays(1, &points);
    glBindVertexArray(points);
    
    glBindBuffer(GL_ARRAY_BUFFER, jointPtr);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);

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
        glDeleteVertexArrays(numJoints, &points);

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
    
    glBindVertexArray(points);
    glDrawArrays(GL_POINTS, 0, numJoints);
    glDrawArrays(GL_LINE_STRIP, 0, numJoints);
    glDrawArrays(GL_LINES_ADJACENCY, 0, numJoints);
}

void SceneMD5::update(const float dt)
{
    SceneBase::update(dt);
}