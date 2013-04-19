#ifndef MD5FRAME
#define MD5FRAME

#include "JointFrame.hpp"


class md5frame
{
private:
    uint numJoints;
    jointframe *joints;
    friend std::ostream& operator<< (std::ostream& os, const md5frame& f);
    friend std::istream& operator>> (std::istream& is, md5frame& f);
    int* hierarchy;
public:
    inline int getParentIndex(uint i) { return hierarchy[i]; }
    md5frame();
    md5frame(int n, int* hier);
    md5frame(const md5frame& rhs);
    ~md5frame();
    inline jointframe getJoint(uint i) const { return joints[i]; }
};

#endif
