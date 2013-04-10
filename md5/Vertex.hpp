/* 
 * File:   Vertex.hpp
 * Author: sandy
 *
 * Created on April 8, 2013, 11:19 AM
 */

#ifndef VERTEX_HPP
#define	VERTEX_HPP

class Vertex 
{
public:
    Vertex();
    Vertex(const Vertex&);
    virtual ~Vertex();
    
    inline const uint getWeightElement() const { return _weightElement; }
    inline void setWeightElement( uint e) { _weightElement = e; }
    
    inline const uint getWeightIndex() const { return _weightIndex; }
    
    inline const uint getIndex() const { return _index; }
    inline void setIndex(const uint index) { _index = index; }
private:
    uint _index;
    uint _weightIndex;
    uint _weightElement;
    glm::vec2 uv;
    friend std::ostream& operator<< (std::ostream&, const Vertex&);
    friend std::istream& operator>> (std::istream&, Vertex&);
};

#endif	/* VERTEX_HPP */