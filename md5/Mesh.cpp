#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#elif  _WIN32
#include <GL/glut.h>
#else
#include <GL/glut.h>
#endif

#include <fstream>

#include "Triangle.hpp"
#include "Vertex.hpp"
#include "Weight.hpp"
#include "Md5Exceptions.hpp"

#include "Mesh.hpp"

mesh::mesh()
    : numverts(0)
    , numtris(0)
    , numweights(0)
    , verts(NULL)
    , tris(NULL)
    , weights(NULL)
{
}

mesh::~mesh()
{
    delete [] verts;
    delete [] tris;
    delete [] weights;
}

std::istream& operator>> (std::istream& imesh, mesh& m)
{
    std::string keyword = "";

    // Materials Area
    do {
        //Eat Junk
        imesh >> keyword;
    } while (keyword != "shader");
    imesh >> m.shader; 

    // Vertices Area
    do {
        //Eat Junk
        imesh >> keyword;
    } while(keyword != "numverts");
    //Num of vertices
    imesh >> m.numverts;
    if(m.numverts == 0)
        throw md5LoadMeshException();

    // Allocate size of vertex array
    m.verts = new vertex[m.numverts];
    for(uint i = 0; i != m.numverts; ++i) {
        imesh >> keyword;
        if(keyword == "vert") {
            imesh >> m.verts[i];
            if(m.verts[i].getIndex() != i) 
                throw md5LoadMeshException();
        } else {
            throw md5LoadMeshException();
        }
    }

    // Triangles area
    do {
        //Eat Junk
        imesh >> keyword;
    } while (keyword != "numtris");
    //Num of triangles
    imesh >> m.numtris;
    if(m.numtris == 0)
        throw md5LoadMeshException();

    // Allocate size of triangle array
    m.tris = new triangle[m.numtris];
    for(uint i = 0; i != m.numtris; ++i) {
        imesh >> keyword;
        if(keyword == "tri") {
            imesh >> m.tris[i];
            if(m.tris[i].getIndex() != i) 
                throw md5LoadMeshException();
        } else {
            throw md5LoadMeshException();
        }
    }

    // Weights area
    do { 
        //Eat Junk
        imesh >> keyword;
    } while(keyword != "numweights");
    imesh >> m.numweights;
    if(m.numweights == 0) 
        throw md5LoadMeshException();

    // Allocate size of weights array
    m.weights = new weight[m.numweights];
    for(uint i = 0; i != m.numweights; ++i) {
        imesh >> keyword;
        if(keyword == "weight") {
            weight w;
            imesh >> w;
            m.weights[i] = w;
            if(w.getIndex() != i)
                throw md5LoadMeshException();
        } else {
            throw md5LoadMeshException();
        }
    }
    return imesh;
}

// Print function
std::ostream& operator<< (std::ostream& os, const mesh& m)
{
    return os << "Mesh with " << m.numverts << " vertices, " 
              << m.numtris << " triangles and " << m.numweights 
              << " weights.";
}

/* Mesh Draw function:
    -------------------
    calls triangle's draw function, which then calls
    the vertex function, which calls weights function,
    that calls bones function */

void mesh::draw(const joint *j)
{
    glColor3f(1.0f,1.0f,1.0f);
    glBegin(GL_TRIANGLES);
    for(uint i = 0; i != numtris; ++i)
        tris[i].draw(verts, weights, j);
    glEnd();
}
