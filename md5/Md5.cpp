#include <string>
#include <fstream>
#include <iostream>
#include <sstream>

#include "Triangle.hpp"
#include "Mesh.hpp"
#include "Joint.hpp"
#include "Md5Frame.hpp"
#include "JointFrame.hpp"
#include "Md5Exceptions.hpp"

#include "Md5.hpp"

md5::md5(std::string s)
    : animTime(0.0f)
    , my_mesh(NULL)
    , my_joints(NULL)
    , my_frames(NULL)
{
    loadmesh(s);
    loadanim(s);
}

md5::~md5()
{
    delete [] my_mesh;
    delete [] my_joints;
    delete [] my_frames;
    delete [] hierarchy;
    my_mesh     = NULL;
    my_joints   = NULL;
    my_frames   = NULL;
    hierarchy   = NULL;
}

std::ostream& operator<< (std::ostream& os, md5& m)
{
    for (uint i = 0; i!= m.numFrames; ++i)
        os << "Frame#" << i << ":\n" << m.my_frames[i];
    return os; 
}

void md5::loadmesh(std::string s)
{
    // Read the Mesh file
    s += ".md5mesh";
    std::ifstream imd5(s.c_str());

    std::string keyword = "";
    while(imd5 >> keyword)
    {
        // Allocate memory for the joints
        if(keyword == "numJoints"){
            imd5 >> numJoints;
            my_joints = new joint[numJoints];
        }
        // Allocate memory for the Meshes
        else if(keyword=="numMeshes"){
            imd5 >> numMeshes;
            my_mesh = new mesh[numMeshes];
        }
        // Read the joints
        else if(keyword == "joints" && imd5 >> keyword){
            if(keyword != "{" ) 
                throw md5LoadMeshException();
            for(uint i = 0; i != numJoints; ++i) 
                imd5 >> my_joints[i];
        }
        // Read the mesh
        else if(keyword == "mesh" && imd5 >> keyword){
            if(keyword != "{" ) 
                throw md5LoadMeshException();
            for(uint i = 0; i != numMeshes; ++i) 
                imd5 >> my_mesh[i];
        }
    } // Read Loop
    imd5.close();
}

void md5::loadanim(std::string s)
{
    std::cout << "starting load anim" << std::endl;
    // Read the Anim file
    s += ".md5anim";
    std::ifstream imd5(s.c_str());
    std::string keyword = "";

    // Eat Junk
    while(imd5 >> keyword && keyword != "numFrames");
    if(keyword != "numFrames") 
        throw md5LoadAnimException();
    imd5 >> numFrames;
    my_frames = new md5frame[numFrames];
    std::cout << "Frame count: " << numFrames << std::endl;

    // Check if the anim corresponds with the mesh
    if(imd5 >> keyword && keyword == "numJoints") {
        int tmpNum = -1;
        imd5 >> tmpNum;
        if (tmpNum != (int)numJoints) 
            throw md5LoadAnimException();
    } else {
        throw md5LoadAnimException();
    }

    if(imd5 >> keyword && keyword == "frameRate") {
        imd5 >> frameRate;
        std::cout << "Frame rate: " << frameRate << std::endl;
    } else {
        throw md5LoadAnimException();
    }

    if(imd5 >> keyword && keyword == "numAnimatedComponents") {
        imd5 >> numAnimatedComponents;
        std::cout << "Animated Components: " << numAnimatedComponents << std::endl;
    } else {
        throw md5LoadAnimException();
    }

    // Joint Hierarchy
    if(!(imd5 >> keyword) || keyword != "hierarchy")
        throw md5LoadAnimException();
    //"[boneName]"   [parentIndex] [numComp] [frameIndex] // ...
    if(!(imd5 >> keyword) || keyword != "{" ) 
        throw md5LoadAnimException();

    getline(imd5,keyword);
    hierarchy = new int[numJoints];
    for(uint i = 0; i != numJoints; ++i) {
        getline(imd5,keyword);
        std::stringstream ss(keyword);
        std::string temp;
        ss >> temp >> hierarchy[i];
    }
    if(!(imd5 >> keyword) || keyword != "}") 
        throw md5LoadAnimException();
    // end of joint Hierarchy


    // Bounds
    if(!(imd5 >> keyword) || keyword != "bounds") 
        throw md5LoadAnimException();
    //( [minX] [minY] [minZ] ) ( [maxX] [maxY] [maxZ] )
    if(!(imd5 >> keyword) || keyword != "{" ) 
        throw md5LoadAnimException();
    getline(imd5,keyword);
    for(uint i = 0; i != numFrames; ++i) {
        getline(imd5,keyword); 
        std::cout << keyword << std::endl;
    }
    if(!(imd5 >> keyword) || keyword != "}") 
        throw md5LoadAnimException();
    // end bounds

    // Base frame
    if(!(imd5 >> keyword) || keyword != "baseframe") 
        throw md5LoadAnimException();
    //( [xPos] [yPos] [zPos] ) ( [xOrient] [yOrient] [zOrient] )
    if(!(imd5 >> keyword) || keyword != "{" )
        throw md5LoadAnimException();
    getline(imd5,keyword);
    // Read Base frame
    for(uint i = 0; i != numJoints; ++i)
        getline(imd5,keyword);
    if(!(imd5 >> keyword) || keyword != "}")
        throw md5LoadAnimException();
    // end of Base frame

    std::cout << "Base frame read, reading frames..." << std::endl;

    // Read in Frames
    for (uint i = 0; i != numFrames; ++i) {
        uint fnum;
        if(!(imd5 >> keyword) || keyword != "frame")
            throw md5LoadAnimException();
        //( [xPos] [yPos] [zPos] ) ( [xOrient] [yOrient] [zOrient] )
        if(!(imd5 >> fnum) || fnum != i)
            throw md5LoadAnimException();
        if(!(imd5 >> keyword) || keyword != "{" )
            throw md5LoadAnimException();

        md5frame* aframe = new md5frame(numJoints, hierarchy);
        imd5 >> *aframe;
        my_frames[i] = *aframe;

        if(!(imd5 >> keyword) || keyword != "}")        
            throw md5LoadAnimException();
    }
    imd5.close();
}

// Top Draw function
void md5::draw()
{
    for(uint i = 0; i != numMeshes; ++i)
    {
        my_mesh[i].draw(my_joints);
        joint::draw(numJoints, my_joints);
    }
}

void md5::animate(float dt)
{
    animTime += dt;
    if (animTime > numFrames)
        animTime -= numFrames;
    uint curframenum = (int)animTime;
    md5frame currentFrame = my_frames[curframenum];
    md5frame nextFrame = my_frames[(curframenum + 1) % numFrames];
    float interp = animTime - curframenum;
    for(uint i = 0; i != numJoints; ++i) {
        if (interpolate)
            my_joints[i].slerp(currentFrame.getJoint(i), nextFrame.getJoint(i), interp);
        else
            my_joints[i].set(currentFrame.getJoint(i));
    }
}
