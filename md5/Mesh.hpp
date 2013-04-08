/* 
 * File:   Mesh.hpp
 * Author: sandy
 *
 * Created on April 8, 2013, 2:39 AM
 */

#ifndef MESH_HPP
#define	MESH_HPP

class Vertex;
class Triangle;
class Weight;

class Mesh 
{
public:
    Mesh();
    Mesh(const Mesh&);
    virtual ~Mesh();
    
    inline const uint getNumVertices() const { return _numVertices; }
    inline const Vertex* getVertices() const { return _vertices; }
    
    inline const uint getNumWeights() const { return _numWeights; }
    inline const Weight* getWeights() const { return _weights; }

private:
    uint _numVertices;
    uint _numTriangles;
    uint _numWeights;
    std::string shader;
    Vertex* _vertices;
    Triangle* _triangles;
    Weight* _weights;
    friend std::ostream& operator<< (std::ostream&, const Mesh&);
    friend std::istream& operator>> (std::istream&, Mesh&);
};

#endif	/* MESH_HPP */
