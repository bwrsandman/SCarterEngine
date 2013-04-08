/* 
 * File:   Triangle.cpp
 * Author: sandy
 * 
 * Created on April 8, 2013, 11:30 AM
 */

#include <fstream>

#include "Triangle.hpp"

Triangle::Triangle()
{
}

Triangle::Triangle(const Triangle& orig)
{
}

Triangle::~Triangle()
{
}

// Triangle reading function
std::istream& operator>> (std::istream& input, Triangle& t)
{
    //tri [triIndex] [vertIndex1] [vertIndex2] [vertIndex3]
    input >> t._index >> t._vertexIndex[0] >> t._vertexIndex[1]
          >> t._vertexIndex[2];

    return input;
}

// Triangle print function
std::ostream& operator<< (std::ostream& output, const Triangle& t)
{
    return output << "Triangle #" << t.getIndex() 
                  << " has the following vertices: " << t._vertexIndex[0] 
                  << ", " << t._vertexIndex[1] << " and " << t._vertexIndex[2];
}