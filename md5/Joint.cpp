/* 
 * File:   Joint.cpp
 * Author: sandy
 * 
 * Created on April 8, 2013, 12:07 AM
 */

#include <string>
#include <iostream>
#include <glm/glm.hpp>

#include "Joint.hpp"

Joint::Joint() {
}

Joint::Joint(const Joint& orig) {
}

Joint::~Joint() {
}

// Print function
std::ostream& operator<< (std::ostream& os, const Joint& j) {
    os << j.getName();
    if(j.getParentIndex() < 0) 
        os << " is root bone ";
    else 
        os << " is son of bone #" << j.getParentIndex();
    os << " at coordinates (" << j.getPosition().x << ", "
                              << j.getPosition().y << ", "
                              << j.getPosition().z << ")\n";
    return os;
}
// Read the joint from file
std::istream& operator>> (std::istream& is, Joint& j) {
    // "[boneName]"   [parentIndex] ( [xPos] [yPos] [zPos] ) ( [xOrient] [yOrient] [zOrient] )
    std::string keyword;
    int parent;
    is >> keyword;
    j.setName(keyword.substr(1, keyword.size() - 2));          // Removes Quotes
    is >> parent >> keyword;                                   // "("
    j.setParentIndex(parent);
    float x, y, z;
    is >> x >> y >> z;													// Reading ( [xPos] [yPos] [zPos] )
    j.setPosition(x,y,z);
    is >> keyword >> keyword; 											// ")", "("
    is >> x >> y >> z;													// Reading ( [xOrient] [yOrient] [zOrient] )
    j.setOrientation(x,y,z);

    getline(is, keyword); 												// Rest of the line
    return is;
}