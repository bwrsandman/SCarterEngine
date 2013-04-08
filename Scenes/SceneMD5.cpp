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

#include "SceneBase.hpp"
#include "SceneMD5.hpp"

SceneMD5::SceneMD5()
{
}

SceneMD5::~SceneMD5()
{
    delete [] joints;
    joints = NULL;
}

void SceneMD5::load_md5_mesh(const char* filename)
{
    std::ifstream input;
    input.open(filename);
    
    std::string keyword;
    int numJoints;
    int numMeshes;
    
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
            for(int i = 0; i != numJoints; ++i)
                input >> joints[i];
        }
        // Read the mesh
        else if (keyword == "mesh" && input >> keyword) {
            assert(keyword == "{" );
            for(int i = 0; i != numMeshes; ++i) 
                input >> meshes[i];
        }
    } // Read Loop
    input.close();
}

void SceneMD5::create_light()
{
}

void SceneMD5::create_geom()
{
}

void SceneMD5::set_perspective()
{
}

void SceneMD5::render(const float dt)
{
    SceneBase::render(dt);
}

void SceneMD5::update(const float dt)
{
    SceneBase::update(dt);
}