#include <fstream>
#include <string>

#include "Quaternion.hpp"
#include "Md5Exceptions.hpp"

#include "Md5Frame.hpp"

md5frame::md5frame()
    : numJoints(0)
    , joints(NULL)
    , hierarchy(NULL)
{
}

md5frame::md5frame(int n, int* h)
    : numJoints(n)
    , joints(new jointframe[numJoints])
    , hierarchy(h)
{
}

md5frame::md5frame(const md5frame& rhs)
    : numJoints(rhs.numJoints)
    , joints(new jointframe[numJoints])
    , hierarchy(NULL)
{
    for(uint i = 0; i!= numJoints; ++i)
        joints[i] = rhs.joints[i];
}

md5frame::~md5frame()
{
    delete [] joints;
    joints = NULL;
}

std::istream& operator>> (std::istream& is, md5frame& f)
{
    // [xPos] [yPos] [zPos] [xOrient] [yOrient] [zOrient]
    std::string keyword = "";
    for (uint i = 0; i != f.numJoints; ++i){
        if(!is) 
            throw md5LoadFrameException();
        jointframe* ajoint = new jointframe(f.getParentIndex(i));
        is >> *ajoint;
        ajoint->toWorld(f);
        f.joints[i]=*ajoint;
    }
    return is;
}

std::ostream& operator<< (std::ostream& os, const md5frame& f) {
    if(f.numJoints == 0){
        os << "empty frame";
        return os;
    }
    for (uint i = 0; i != f.numJoints; ++i) {
        os << f.joints[i] << "\n";
    }

    return os;
}
