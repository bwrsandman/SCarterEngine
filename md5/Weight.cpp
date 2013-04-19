#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#elif  _WIN32
#include <GL/glut.h>
#else
#include <GL/glut.h>
#endif

#include <fstream>

#include "Quaternion.hpp"
#include "Joint.hpp"
#include "Md5Exceptions.hpp"

#include "Weight.hpp"

weight::weight()
    : weightIndex(-1)
    , jointIndex(-1)
    , weightValue(-1.0f)
{
}

weight::~weight()
{
}

// Weight reading function
std::istream& operator>> (std::istream& is, weight& w)
{
    // weight [weightIndex] [jointIndex] [weightValue] ( [xPos] [yPos] [zPos] )
    char tmp = '\0';
    is >> w.weightIndex >> w.jointIndex >> w.weightValue;
    // must be '('
    is >> tmp;
    if(tmp != '(') 
        throw md5LoadMeshException();
    is >> w.pos.x >> w.pos.y >> w.pos.z;
    is >> tmp;
    if(tmp != ')') 
        throw md5LoadMeshException();
    return is;
}

// Weight printing function
std::ostream& operator<< (std::ostream& os, const weight& w)
{
    return os << "Weight#" << w.weightIndex << " is tied to joint " 
              << w.jointIndex << " with weight of " << w.weightValue 
              << " and position (" << w.pos.x << ", " << w.pos.y << ", " 
              << w.pos.z << ")";
}

//Global Position
glm::vec3 weight::getPos(const joint *j) const
{
    uint colIndex = jointIndex > 2? 2 : jointIndex;
    glColor3f(outColors[colIndex].r, outColors[colIndex].g, outColors[colIndex].b);

    glm::vec3 result = j[jointIndex].getOrient().rotate(pos);
    // Translate and apply weight
    result = (result + j[jointIndex].getPos()) * weightValue;
    return result;
}

// Called by vertex, calls bone and quaternions
void weight::draw(int numweights, const weight *w, const joint *j)
{
    for (int i = 0; i != numweights; ++i){
        glColor3f(0.0f, 1.0f, 0.0f);
        glPointSize(3);
        glBegin(GL_POINTS);

        glm::vec3 gcoord = w[i].getPos(j);

        glVertex3f(gcoord.x, gcoord.y, gcoord.z);
        glEnd();
    }
}
