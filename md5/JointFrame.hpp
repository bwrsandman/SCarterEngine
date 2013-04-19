#ifndef JOINTFRAME
#define JOINTFRAME

#include <glm/glm.hpp>

class md5frame;
class quaternion;

class jointframe
{
private:
    friend std::ostream& operator<< (std::ostream& os, const jointframe& jf);
    friend std::istream& operator>> (std::istream& is, jointframe& jf);
    glm::vec3 pos;
    quaternion *orient;
    int parentIndex;    // -1 if it is the root

public:
    jointframe();
    jointframe(int parentIndex);
    ~jointframe();
    jointframe(const jointframe& rhs);
    jointframe& operator=(const jointframe& rhs);
    inline glm::vec3 getPos() const { return pos; }
    void toWorld(const md5frame& f);
    quaternion getOrient() const;
};

#endif