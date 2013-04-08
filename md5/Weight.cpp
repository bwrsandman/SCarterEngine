/* 
 * File:   Weight.cpp
 * Author: sandy
 * 
 * Created on April 8, 2013, 11:41 AM
 */

#include <fstream>
#include <assert.h>
#include <glm/glm.hpp>

#include "Weight.hpp"

Weight::Weight() {
}

Weight::Weight(const Weight& orig) {
}

Weight::~Weight() {
}

/*============================
    Weights Class
============================*/

// Weight reading function
std::istream& operator>> (std::istream& input, Weight& w)
{
    // weight [weightIndex] [jointIndex] [weightValue] ( [xPos] [yPos] [zPos] )
    char tmp = 0;
    input >> w._index >> w._jointIndex >> w._value;
    
    input >> tmp;
    assert (tmp == '(') ;
    
    float x, y, z;
    input >> x >> y >> z;
    w._position = glm::vec3(x, y, z);
    
    input >> tmp;																// must be ')';
    assert (tmp == ')');
    
    return input;
}

// Weight printing function
std::ostream& operator<< (std::ostream& output, const Weight& w)
{
    return output << "Weight #" << w.getIndex() << " is tied to joint " 
                  << w._jointIndex << " with weight of " << w._value 
                  << " and position (" << w._position.x << ", " 
                  << w._position.y << ", " << w._position.z << ")";
}
