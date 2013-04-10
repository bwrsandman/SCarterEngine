/* 
 * File:   Joint.hpp
 * Author: sandy
 *
 * Created on April 8, 2013, 12:07 AM
 */

#ifndef JOINT_HPP
#define	JOINT_HPP

#include <string>
#include <glm/glm.hpp>

class Joint {
public:
    Joint();
    Joint(const Joint& orig);
    virtual ~Joint();
    
    inline const std::string getName() const { return _name; }
    inline void setName(std::string name) { _name = name; }
    
    inline const int getParentIndex() const { return _parent; }
    inline void setParentIndex(int parent) { _parent = parent; }
    
    inline const glm::vec3 getPosition() const { return _position; }
    inline void setPosition(float x, float y, float z) 
        { _position = glm::vec3(x, y, z); }
    
    inline const glm::vec4 getOrientation() const { return _orientation;}
    
    inline void setOrientation(float x, float y, float z) 
    { 
        float w = 1.0f - x * x - y * y  - z * z;
        _orientation.w = (w < 0.0f)? 0.0f : -sqrt (w);
        _orientation.x = x;
        _orientation.y = y;
        _orientation.z = z;
    }
private:
    friend std::ostream& operator<< (std::ostream&, const Joint&);
    friend std::istream& operator>> (std::istream&, Joint&);
    std::string _name;
    int _parent;
    glm::vec3 _position;
    glm::vec4 _orientation;     // This bone's WXYZ orientation quaternion.
};

#endif	/* JOINT_HPP */