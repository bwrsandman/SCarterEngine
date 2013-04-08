/* 
 * File:   Mesh.cpp
 * Author: sandy
 * 
 * Created on April 8, 2013, 2:39 AM
 */

#include <fstream>
#include <vector>
#include <assert.h>

#include "Joint.hpp"
#include "Mesh.hpp"
#include "Vertex.hpp"
#include "Triangle.hpp"
#include "Weight.hpp"

Mesh::Mesh() {
}

Mesh::Mesh(const Mesh& orig) {
}

Mesh::~Mesh() {
    delete [] _vertices;
    delete [] _triangles;
    delete [] _weights;
    _vertices = NULL;
    _triangles = NULL;
    _weights = NULL;
}

std::istream& operator>>(std::istream& input, Mesh& m) {
    std::string keyword;

    // Materials Area
    do {
        input >> keyword;
    } while (keyword != "shader");
    input >> m.shader;

    // Vertices Area
    do {
        input >> keyword;
    } while (keyword != "numverts");
    input >> m._numVertices;

    assert(m._numVertices > 0);

    // Allocate size of vertex array
    m._vertices = new Vertex[m._numVertices];
    for (uint i = 0; i != m._numVertices; ++i) {
        input >> keyword;
        assert (keyword == "vert");
        input >> m._vertices[i];
        assert (m._vertices[i].getIndex() == i);
    }

    // Triangles area
    do {
        input >> keyword;
    } while (keyword != "numtris");
    input >> m._numTriangles;
    assert (m._numTriangles > 0);

    // Allocate size of triangle array
    m._triangles = new Triangle[m._numTriangles];
    for (uint i = 0; i != m._numTriangles; ++i) {
        input >> keyword;
        assert (keyword == "tri");
        input >> m._triangles[i];
        assert (m._triangles[i].getIndex() == i);
    }

    // Weights area
    do {
        input >> keyword;
    } while (keyword != "numweights");
    input >> m._numWeights;
    assert (m._numWeights > 0);

    std::vector<Weight> weights;
    
    int uselessIndex = 0;
    int uselessWeights;
    for (uint i = 0; i != m._numVertices; ++i) {
        m._vertices[i].setIndex(m._vertices[i].getIndex() - uselessIndex);
        uselessWeights = 0;
        for (uint j = 0; j != m._vertices[i].getWeightElement(); ++j) {
            input >> keyword;
            assert (keyword == "weight");
            Weight w;
            input >> w;
            if (w.isSignificant())
                weights.push_back(w);
            else
                ++uselessWeights;
        }
        m._vertices[i].setWeightElement(m._vertices[i].getWeightElement() 
                                        - uselessWeights);
        uselessIndex += uselessWeights;
    }
    m._weights = weights.data();
    //std::cout << "Removed " << totuselessw 
    //          << " useless weights: Vector is now of size:" 
    //          << m.weights.size() << endl;
    return input;
}

// Print function
// --------------

std::ostream& operator<<(std::ostream& ouput, const Mesh& m) {
    return ouput << "Mesh with " << m._numVertices << " vertices, "
            << m._numTriangles << " triangles and " << m._numWeights
            << " weights.";
}