/* 
 * File:   IKArm.hpp
 * Author: sandy
 *
 * Created on February 18, 2013, 1:29 PM
 */

#ifndef IKARM_HPP
#define	IKARM_HPP

class IKLine;
class Matrix2x3f;
class Matrix3x2f;
class Matrix2f;
class Vector2f;

class IKArm {
public:
    IKArm();
    IKArm(const IKArm& orig);
    virtual ~IKArm();
    IKLine *lines;
    void move_to(Vector2f, float);
private:
    const float max_vel = 0.5f;
    float dXdAlpha() const;
    float dYdAlpha() const;
    float dXdBeta() const;
    float dYdBeta() const;
    float dXdGamma() const;
    float dYdGamma() const;
    Matrix2x3f Jacobian() const;
    Matrix3x2f Jtransp() const;
    Matrix2f JJtransp() const;
    Matrix2f invJJtransp() const;
    Matrix3x2f Jplus() const;
    void move_by(Vector2f);
    inline float alpha() const { return loc_orient(0); }
    inline float beta()  const { return loc_orient(1); }
    inline float gamma() const { return loc_orient(2); }
    float loc_orient(int i) const;
    inline float A() const { return len(0); }
    inline float B() const { return len(1); }
    inline float C() const { return len(2); }
    float len(int i) const;
};

#endif	/* IKARM_HPP */

