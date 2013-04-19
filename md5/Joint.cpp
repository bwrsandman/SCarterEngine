#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#elif  _WIN32
#include <GL/glut.h>
#else
#include <GL/glut.h>
#endif


#include <fstream>
#include <string>

#include "Triangle.hpp"
#include "JointFrame.hpp"

#include "Joint.hpp"

joint::joint()
{
}

joint::~joint()
{
    delete orient;
    orient = NULL;
}

// Print function
std::ostream& operator<< (std::ostream& os, const joint& j)
{
    os << j.boneName;
    if(j.parentIndex < 0)
        os << " is root bone ";
    else
        os << "is son of bone#" << j.parentIndex;
    os << " at coordinates (" << j.pos.x << ", " 
       << j.pos.y << ", " << j.pos.z;
    return os;
}

// Read the joint from file
std::istream& operator>> (std::istream& is, joint& j)
{
    //"[boneName]"   [parentIndex] ( [xPos] [yPos] [zPos] ) ( [xOrient] [yOrient] [zOrient] )
    std::string keyword = "";
    is >> keyword;
    // Removes Quotes
    j.boneName = keyword.substr(1, keyword.size() - 2);
    // "("
    is >> j.parentIndex >> keyword;
    float x, y, z;
    is >> x >> y >> z;
    j.pos = glm::vec3(x, y, z);
    // ")", "("
    is >> keyword >> keyword;
    is >> x >> y >> z;
    j.orient = new quaternion(x,y,z);
    // Read the rest of the line 
    getline(is, keyword);
    return is;
}

void joint::set(const jointframe& jf)
{
    *orient = jf.getOrient();
    pos = jf.getPos();
}

// Joints Drawing functions, for debugging
void joint::draw(int numjoints, const joint *j)
{
    for (int i = 0; i != numjoints; ++i){
        glm::vec3 pos = j[i].getPos();
        glColor3f(1.0f, 0.0f, 0.0f);
        glBegin(GL_LINES);
            int parent = j[i].parentIndex;
            if(parent >= 0){
                glm::vec3 parentPos = j[parent].getPos();
                glVertex3f(pos.x, pos.y, pos.z);
                glVertex3f(parentPos.x, parentPos.y, parentPos.z);
            }
        glEnd();

        glColor3f(1.0f, 1.0f, 1.0f);
        glPointSize(5.0);

        if(j[i].parentIndex == -1)
            glPointSize(10.0);

        glBegin(GL_POINTS);
            glVertex3f(pos.x, pos.y, pos.z);
        glEnd();
    }
}
