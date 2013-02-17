/*

	Copyright 2010 Etay Meiri

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef MATH_3D_H
#define	MATH_3D_H


struct Vector3f
{
    float x;
    float y;
    float z;

    Vector3f()
    {
    }

    Vector3f(float _x, float _y, float _z)
    {
        x = _x;
        y = _y;
        z = _z;
    }
};

struct Quad3f
{
	/**********************
	 *
	 *  v1-----v2
	 *  |       |
	 *  |       |
	 *  |       |
	 *  v0-----v3
	 *
	 **********************/
	Vector3f v[4];
	
	Quad3f()
	{
	}

	Quad3f(Vector3f v0, Vector3f v1, Vector3f v2, Vector3f v3)
	{
		v[0] = v0;
		v[1] = v1;
		v[2] = v2;
		v[3] = v3;
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

