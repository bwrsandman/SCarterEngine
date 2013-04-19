#ifndef JOINT
#define JOINT

#include <glm/glm.hpp>

#include "Quaternion.hpp"

class jointframe;

class joint
{
private:
    friend std::ostream& operator<< (std::ostream& os, const joint& j);
    friend std::istream& operator>> (std::istream& is, joint& j);
    std::string boneName;   // - The name of this bone.
    int parentIndex;        // - The index of this bone's parent. -1 if it is the root
    glm::vec3 pos;          // - This bone's XYZ position.
    quaternion *orient;     // - This bone's WXYZ orentation quaternion.

public:
    joint();
    ~joint();

    inline glm::vec3 getPos() const { return pos; }
    inline quaternion getOrient() const { return *orient; }

    void set(const jointframe& jf); 
    void slerp(const jointframe&, const jointframe&, float);
    static void draw(int numjoints, const joint *j);

};

#endif
