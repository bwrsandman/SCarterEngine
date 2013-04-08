/* 
 * File:   Triangle.hpp
 * Author: sandy
 *
 * Created on April 8, 2013, 11:30 AM
 */

#ifndef TRIANGLE_HPP
#define	TRIANGLE_HPP

class Triangle 
{
public:
    Triangle();
    Triangle(const Triangle&);
    virtual ~Triangle();
    inline const uint getIndex() const { return _index; }
private:
    uint _index;
    uint _vertexIndex[3];
    friend std::ostream& operator<< (std::ostream&, const Triangle&);
    friend std::istream& operator>> (std::istream&, Triangle&);
};

#endif	/* TRIANGLE_HPP */

