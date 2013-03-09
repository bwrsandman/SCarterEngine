#ifndef MATH_3D_H
#define	MATH_3D_H

class Vector2f
{
public:
    const float X;
    const float Y;
    
    Vector2f(float x=0.0f, float y=0.0f)
        : X(x)
        , Y(y)
    {
    }
    
     Vector2f(const Vector2f& orig)
        : X(orig.X)
        , Y(orig.Y)
    {
    }
    
    float mag_squaded()
    {
        return X * X + Y * Y;
    }
    
    float magnitude()
    {
        return sqrt(mag_squaded());
    }
    
    Vector2f normalized()
    {
        return 1.0f/magnitude() * (*this);
    }
    
    Vector2f operator+(const Vector2f& rhs) const
    {
        return Vector2f(this->X + rhs.X, this->Y + rhs.Y);
    }
    
    Vector2f operator-(const Vector2f& rhs) const
    {
        return Vector2f(this->X - rhs.X, this->Y - rhs.Y);
    }

    Vector2f operator*(float rhs) const
    {
        return Vector2f(this->X * rhs, this->Y * rhs);
    }
    
    friend Vector2f operator* (double lhs, const Vector2f& rhs)
    {
        return rhs * lhs;
    }
    
    float operator[](int i) const
    {
        return ((i % 2)? Y: X);
    }
};

class Vector3f
{
public:
    const float X;
    const float Y;
    const float Z;

    Vector3f(float x=0.0f, float y=0.0f, float z=0.0f)
        : X(x)
        , Y(y)
        , Z(z)
    {
    }
};

class Matrix2f
{
public:
    const float m[4];
    Matrix2f (float e00, float e01,
              float e10, float e11)
        : m {e00, e01, e10, e11} 
    {
    }
        
    float determinant()
    {
        return m[0] * m[3] - m[1] * m[2];
    }
        
    Matrix2f inverse()
    {
        float delta = determinant();
        if (abs(delta) > 0.01f)
            return 1.0f/delta * Matrix2f ( m[3], -m[1],
                                          -m[2],  m[0]);
        else //TODO Singularity
            return *this;
    }
    
    float operator[](int i) const
    {
        return m[i % 4];
    }
    
    Matrix2f operator* (float rhs) const
    {
    /*****************************************
     * 
     * [ l[0], l[1] ]  [ r[0], r[1] ]
     * [ l[2], l[3] ]  [ r[2], r[3] ]
     * 
     *****************************************/
        const float *l = m;
        return Matrix2f( l[0] * rhs, l[1] * rhs,
                         l[2] * rhs, l[3] * rhs);
    }
    
    friend Matrix2f operator* (float l, const Matrix2f &r)
    {
        return r * l;
    }
        
    Matrix2f operator* (const Matrix2f &r)
    {
    /*****************************************
     * 
     * [ l[0], l[1] ]  [ r[0], r[1] ]
     * [ l[2], l[3] ]  [ r[2], r[3] ]
     * 
     *****************************************/
        const float *l = m;
        return Matrix2f(
              l[0]*r[0] + l[1]*r[2], l[0]*r[1] + l[1]*r[3],
              l[2]*r[0] + l[3]*r[2], l[2]*r[1] + l[3]*r[3]);
    }
};

class Matrix3x2f
{
    /**************************
     * 
     *  [ m[0], m[1] ]
     *  [ m[2], m[3] ]
     *  [ m[4], m[5] ] 
     * 
     **************************/
public:
    const float m[6];
    Matrix3x2f (float e00, float e01,
                float e10, float e11,
                float e20, float e21)
        : m {e00, e01, e10, e11, e20, e21} 
    {
    }
   
    float operator[](int i) const
    {
        return m[i % 6];
    }
    
    Matrix3x2f operator* (const Matrix2f &r)
    {
    /*****************************************
     * 
     * [ l[0], l[1] ]
     * [            ]  [ r[0], r[1] ]
     * [ l[2], l[3] ]  [            ]
     * [            ]  [ r[2], r[3] ]
     * [ l[4], l[5] ]
     * 
     *****************************************/
        const float *l = m;
        return Matrix3x2f(
              l[0]*r[0] + l[1]*r[2], l[0]*r[1] + l[1]*r[3],
              l[2]*r[0] + l[3]*r[2], l[2]*r[1] + l[3]*r[3],
              l[4]*r[0] + l[5]*r[2], l[4]*r[1] + l[5]*r[3]);
    }
    
    Vector3f operator*(Vector2f r) const
    {
    /*****************************************
    * 
    * [ l[0], l[1] ]
    * [ l[2], l[3] ]  [ r[0], r[1] ]
    * [ l[4], l[5] ]
    * 
    *****************************************/
        const float *l = m;
        return Vector3f(
              l[0]*r[0] + l[1]*r[1],
              l[2]*r[0] + l[3]*r[1],
              l[4]*r[0] + l[5]*r[1]);
    }
};

class Matrix2x3f
{
    /**************************
     * 
     *  [ m[0], m[1], m[2] ]
     *  [ m[3], m[4], m[5] ] 
     * 
     **************************/
public:
    const float m[6];
    Matrix2x3f (float e00, float e01, float e02,
                float e10, float e11, float e12)
        : m {e00, e01, e02, e10, e11, e12} 
    {
    }
        
    Matrix3x2f transpose()
    {
        //TODO
        return Matrix3x2f(m[0], m[3], 
                          m[1], m[4], 
                          m[2], m[5]);
    }
    
    float operator[](int i) const
    {
        return m[i % 6];
    }
    
    Matrix2f operator* (const Matrix3x2f &r)
    {
    /*****************************************
     * 
     *                       [ r[0], r[1] ]
     * [ l[0], l[1], l[2] ]  [            ]
     * [ l[3], l[4], l[5] ]  [ r[2], r[3] ]
     *                       [            ]
     *                       [ r[4], r[5] ]
     * 
     *****************************************/
        const float *l = m;
        return Matrix2f(
              l[0]*r[0] + l[1]*r[2] + l[2]*r[4], l[0]*r[1] + l[1]*r[3] + l[2]*r[5],
              l[3]*r[0] + l[4]*r[2] + l[5]*r[4], l[3]*r[1] + l[4]*r[3] + l[5]*r[5]);
    }
    
};

struct Matrix4f
{
    float m[16];
    Matrix4f (float e00, float e01, float e02, float e03,
              float e10, float e11, float e12, float e13,
              float e20, float e21, float e22, float e23,
              float e30, float e31, float e32, float e33)
    {
        m[0]  = e00;  m[1] = e01; m[2]  = e02; m[3]  = e03;
        m[4]  = e10;  m[5] = e11; m[6]  = e12; m[7]  = e13;
        m[8]  = e20;  m[9] = e21; m[10] = e22; m[11] = e23;
        m[12] = e30; m[13] = e31; m[14] = e32; m[15] = e33;
    }
};


#endif	/* MATH_3D_H */
