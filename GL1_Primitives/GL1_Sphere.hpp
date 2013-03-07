/* 
 * File:   GL1_Sphere.hpp
 * Author: sandy
 *
 * Created on March 7, 2013, 3:27 PM
 */

#ifndef GL1_SPHERE_HPP
#define	GL1_SPHERE_HPP

class Point;

const float sqrt_point_nine = 0.9486832980505137f;

void drawSphere(double, int, int);

class GL1_Sphere {
public:
    GL1_Sphere(float radius, int lats=20, int longs=20, 
           float x=0.0f, float y=0.0f, float z=0.0f);
    GL1_Sphere(float, float, float, float);
    GL1_Sphere(float, Point);
    GL1_Sphere(const GL1_Sphere& orig);
    virtual ~GL1_Sphere();
    
    inline float right(void) const  { return _px + _radius; }
    inline float left(void) const   { return _px - _radius; }
    inline float top(void) const    { return _py + _radius; }
    inline float bottom(void) const { return _py - _radius; }
    inline float front(void) const  { return _pz + _radius; }
    inline float back(void) const   { return _pz - _radius; }
    inline float mass(void) const   { return _mass; }
    // TODO use vector
    float speed(void) const ;
    void put_at(Point p);
    
    inline float vx(void) const     { return _vx; }
    inline float vy(void) const     { return _vy; }
    inline float vz(void) const     { return _vz; }
    inline void fx(float fx)        { _fx += fx; }
    inline void fy(float fy)        { _fy += fy; }
    inline void fz(float fz)        { _fz += fz; }
    
    void draw(void);
    void update(float);
    void clear_forces(void);
    void change_direction(unsigned char, float, bool);
    
private:
    float _radius;
    int _lats;
    int _longs;

    float _px =  0.0f;
    float _py =  0.0f;
    float _pz =  0.0f;

    float _vx =  1.0f;
    float _vy =  1.0f;
    float _vz = -2.0f;

    float _fx =  0.0f;
    float _fy =  0.0f;
    float _fz =  0.0f;

    float _mass = 1.0f;

};

#endif	/* GL1_SPHERE_HPP */

