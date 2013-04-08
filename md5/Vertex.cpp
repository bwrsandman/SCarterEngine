/* 
 * File:   Vertex.cpp
 * Author: sandy
 * 
 * Created on April 8, 2013, 11:19 AM
 */

#include <fstream>
#include <assert.h>
#include <glm/glm.hpp>

#include "Vertex.hpp"

Vertex::Vertex()
{
}

Vertex::Vertex(const Vertex& orig)
{
}

Vertex::~Vertex()
{
}

std::istream& operator>> (std::istream& input, Vertex& v)
{
    //vert [vertIndex] ( [texU] [texV] ) [weightIndex] [weightElem]
    char tmp = 0;
    input >> v._index;
    
    input >> tmp;
    assert (tmp == '(');
    
    float texU, texV;
    input >> texU >> texV;
    v.uv = glm::vec2(texU, texV);
    
    input >> tmp;
    assert (tmp == ')');
    
    input >> v._weightIndex >> v._weightElement;
    return input;
}

// Vertex Print function
std::ostream& operator<< (std::ostream& output, const Vertex& v)
{
    return output << "Vertex #" << v._index << ":\tUV = (" << v.uv.x << ", " 
                  << v.uv.y << "),\tWeight Index: " << v._weightIndex 
                  << ", Weight Element: " << v._weightElement;
}