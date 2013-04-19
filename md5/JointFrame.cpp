#include <fstream>
#include <glm/glm.hpp>

#include "Quaternion.hpp"
#include "Md5Frame.hpp"
#include "Md5Exceptions.hpp"

#include "JointFrame.hpp"

jointframe::jointframe()
    : orient(NULL)
    , parentIndex(-2)
{
}

jointframe::jointframe(int p)
    : orient(NULL)
    , parentIndex(p)
{
}

jointframe::jointframe(const jointframe& rhs)
{
    pos = rhs.pos;
    orient = new quaternion(*rhs.orient);
    parentIndex = rhs.parentIndex;
}

jointframe::~jointframe()
{
    delete orient;
}

jointframe& jointframe::operator=(const jointframe& rhs)
{
    pos = rhs.pos;
    delete orient;
    orient = new quaternion(*rhs.orient);
    parentIndex = rhs.parentIndex;
    return *this;
}

std::istream& operator>> (std::istream& is, jointframe& jf){
    // [xPos] [yPos] [zPos] [xOrient] [yOrient] [zOrient]
    float x,y,z;
    is >> x >> y >> z;
    jf.pos = glm::vec3(x,y,z);
    is >> x >> y >> z;
    jf.orient = new quaternion(x,y,z);
    return is;
}

std::ostream& operator<< (std::ostream& os, const jointframe& jf)
{
    return os << "\tparent: " << jf.parentIndex << " pos(x,y,z): " 
              << "(" << jf.pos.x << "," << jf.pos.y << "," << jf.pos.z 
              << ")" << "\torient(w, x,y,z): " << jf.getOrient();
}

quaternion jointframe::getOrient() const
{
    if (orient == NULL) 
        throw md5JointException();
    return quaternion(*orient);
}

// Change the pos and orient to world space using parent
void jointframe::toWorld(const md5frame& f)
{
    if(parentIndex==-2) 
        throw md5JointException();
    if (orient == NULL) 
        throw md5JointException();
    if(parentIndex >= 0) {
        jointframe parent = f.getJoint(parentIndex);
        (*orient) = *(parent.orient) * (*orient);
        orient->normalize();
        pos = parent.pos + parent.orient->rotate(pos);
    }
}
