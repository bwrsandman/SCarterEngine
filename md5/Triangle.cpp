#include <fstream>

#include "Vertex.hpp"
#include "Md5Exceptions.hpp"

#include "Triangle.hpp"

triangle::triangle()
    : triIndex(-1)
    , vertIndex1(-1)
    , vertIndex2(-1)
    , vertIndex3(-1)
{
}

triangle::~triangle()
{
}

// Triangle reading function
std::istream& operator>> (std::istream& is, triangle& t)
{
    //tri [triIndex] [vertIndex1] [vertIndex2] [vertIndex3]
    is >> t.triIndex >> t.vertIndex1 >> t.vertIndex2 >> t.vertIndex3;

    if(t.triIndex == -1)
        throw md5LoadMeshException();
    if(t.vertIndex1 == -1 || t.vertIndex2 == -1 || t.vertIndex3 == -1 )
        throw md5LoadMeshException();
    return is;
}

// Triangle print function
std::ostream& operator<< (std::ostream& os, const triangle& t)
{
    return os << "Triangle#" << t.triIndex << " has the following vertices: " 
              << t.vertIndex1 <<", " << t.vertIndex2 << " and " << t.vertIndex3;
}

// Called by Mesh function, call vertex's draw
void triangle::draw(const vertex *v, const weight *w, const joint *j)
{
    if(vertIndex1 < 0 || vertIndex2 < 0 || vertIndex3 < 0 )
        throw md5DrawMeshException();

    v[vertIndex1].draw(w,j);
    v[vertIndex2].draw(w,j);
    v[vertIndex3].draw(w,j);
}
