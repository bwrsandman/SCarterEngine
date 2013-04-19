#ifndef QUATERNION
#define QUATERNION

#include <glm/glm.hpp>

class quaternion
{
private:
    float w;
    glm::vec3 v;

    quaternion(float, glm::vec3);

    friend const quaternion operator*(const quaternion&, const float&);
    friend const quaternion operator*(const float&, const quaternion&);
    friend const quaternion operator*(const quaternion&, const quaternion&);
    friend const quaternion operator*(const quaternion&, const glm::vec3&);
    friend const quaternion operator*(const glm::vec3&, const quaternion&);
    friend const quaternion operator-(const quaternion&);
    friend std::ostream& operator<<(std::ostream&, const quaternion&); 

    template <typename T>
    const quaternion operator*=(const T&);  

    static float makew(float, float, float);

public:
    void normalize();
    quaternion();
    quaternion(glm::vec3);
    quaternion(float, float, float);
    quaternion(const quaternion&);
    quaternion& operator=(const quaternion&);
    ~quaternion();

    glm::vec3 rotate(const glm::vec3&) const;
};

#endif