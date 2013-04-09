/* 
 * File:   Weight.hpp
 * Author: sandy
 *
 * Created on April 8, 2013, 11:41 AM
 */

#ifndef WEIGHT_HPP
#define	WEIGHT_HPP

class Weight 
{
public:
    Weight();
    Weight(const Weight&);
    virtual ~Weight();
    inline const uint getIndex() const { return _index; }
    inline const uint getJointIndex() const { return _jointIndex; }
    inline const glm::vec3 getPosition() const { return _position; }
    inline const float getValue() const { return _value; }
    inline const bool isSignificant() const { return _value; }
private:
    uint _index;
    uint _jointIndex;
    float _value;
    glm::vec3 _position;
    friend std::ostream& operator<< (std::ostream&, const Weight&);
    friend std::istream& operator>> (std::istream&, Weight&);
};

#endif	/* WEIGHT_HPP */

