#include <fstream>
#include <glm/glm.hpp>

#include "Quaternion.hpp"

quaternion::quaternion()
    : w(0.0f)
    , v(glm::vec3(0.0f))
{
}

quaternion::quaternion(float x, float y, float z)
    : w(makew(x, y, z))
    , v(glm::vec3(x, y, z))
{
}

quaternion::quaternion(glm::vec3 v)
    : w(makew(v.x, v.y, v.z))
    , v(v)
{
}

quaternion::quaternion(float w, glm::vec3 v)
    : w(w)
    , v(v)
{
}

quaternion::quaternion(const quaternion& rhs) // Copy constructor
    : w(rhs.w)
    , v(rhs.v)
{
}

quaternion& quaternion::operator=(const quaternion& rhs)
{
    w = rhs.w;
    v = rhs.v;
    return *this;
}

quaternion::~quaternion()
{
}


// Create w from the file using this relation 1^2 = w^2 + x^2 + y^2 + z^2
float quaternion::makew(float x, float y, float z)
{
    float s = 1 - x * x - y * y - z * z;
    return (s<0)? -sqrt(-s) : -sqrt(s);
}

template <typename T>
const quaternion quaternion::operator*=(const T& rhs)
{
    *this = *this * rhs;
    return *this;
}

const quaternion operator-(const quaternion& q)
{ 
    return -1.0f * q; 
}

const quaternion operator*(const float& lhs, const quaternion& rhs)
{ 
    return rhs * lhs; 
}

const quaternion operator*(const quaternion& lhs, const float& rhs)
{
    //perform scalar multiplication
    float s = lhs.w * rhs;
    glm::vec3 c = lhs.v * rhs;
    quaternion result = quaternion(s, c);
    return result;
}

const quaternion operator*(const quaternion& lhs, const quaternion& rhs)
{
    //perform quarternion multiplication
    float s, x, y, z;
    s = lhs.w * rhs.w   - lhs.v.x * rhs.v.x - lhs.v.y * rhs.v.y - lhs.v.z * rhs.v.z; //(w1w2 - x1x2 - y1y2 - z1z2)
    x = lhs.w * rhs.v.x + lhs.v.x * rhs.w   + lhs.v.y * rhs.v.z - lhs.v.z * rhs.v.y; //(w1x2 + x1w2 + y1z2 - z1y2)
    y = lhs.w * rhs.v.y - lhs.v.x * rhs.v.z + lhs.v.y * rhs.w   + lhs.v.z * rhs.v.x; //(w1y2 - x1z2 + y1w2 + z1x2)
    z = lhs.w * rhs.v.z + lhs.v.x * rhs.v.y - lhs.v.y * rhs.v.x + lhs.v.z * rhs.w;   //(w1z2 + x1y2 - y1x2 + z1w2)

    return quaternion(s, glm::vec3(x,y,z));
}

const quaternion operator*(const quaternion& lhs, const glm::vec3& rhs)
{
    // perform quaternion multiplication with w = 0.0f
    return lhs * quaternion(0.0f, rhs);
}

const quaternion operator*(const glm::vec3& lhs, const quaternion& rhs)
{
    // perform quaternion multiplication with w = 0.0f
    return quaternion(0.0f, lhs) * rhs;
}

void quaternion::normalize()
{
    float mag = w * w + v.x * v.x + v.y * v.y + v.z * v.z;
    // calculate the divide ratio once, check for division by zero;
    mag = (mag)? 1.0f / mag : 1.0f;
    *this *= mag;
}

glm::vec3 quaternion::rotate(const glm::vec3& c) const
{
    //Perform rotation
    // compute q^-1
    quaternion inverse = quaternion(w, (-(*this)).v);
    inverse.normalize();
    quaternion result = (*this) * c * inverse;
    return result.v;
}

// print function
std::ostream& operator<<(std::ostream& os, const quaternion& q)
{
    os << "(" << q.w << ", " << q.v.x << ", " << q.v.y << ", " << q.v.z << ")";
    return os;
}
