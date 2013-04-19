#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#elif  _WIN32
#include <GL/glut.h>
#else
#include <GL/glut.h>
#endif

#include <glm/glm.hpp>
#include <fstream>

#include "Weight.hpp"
#include "Md5Exceptions.hpp"

#include "Vertex.hpp"

// Vertex Read function
std::istream& operator>> (std::istream& is, vertex& v)
{
    //vert [vertIndex] ( [texU] [texV] ) [weightIndex] [weightElem]
    char tmp = '\0';
    is >> v.vertIndex;
    // must be '('
    is >> tmp;
    if(tmp != '(') 
        throw md5LoadMeshException();
    is >> v.texU >> v.texV;
    // must be ')'
    is >> tmp;
    if(tmp != ')')
        throw md5LoadMeshException();
    is >> v.weightIndex >> v.weightElem;
    return is;
}

// Vertex Print function
std::ostream& operator<< (std::ostream& os, const vertex& v)
{
    return os << "Vertex#" << v.vertIndex << ":\tUV = (" << v.texU 
              << ", " << v.texV << "),\tWeight Index: " << v.weightIndex 
              << ", Weight Element: " << v.weightElem;
}

vertex::vertex()
    : vertIndex(-1)
    , weightIndex(-1)
    , texU(-1.0f)
    , texV(-1.0f)
{
}

vertex::~vertex()
{
}

// Called by triangle, calls weight's draw
void vertex::draw(const weight *w, const joint *j) const
{
    glm::vec3 final(0.0f);
    for(int i = 0; i != weightElem; ++i){
        glm::vec3 weight = w[weightIndex + i].getPos(j);
        final += weight;
    }
    glVertex3f(final.x, final.y, final.z);

}