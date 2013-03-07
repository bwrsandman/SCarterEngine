#ifndef INTERPOLATION
#define INTERPOLATION

#include <GL/gl.h>
#include <assert.h>

float 
cubic_interpolate1 (float y0, float y1, float y2, float y3, float p)
{
   float p2 =  p * p;
   float a0 = -0.5f * y0 + 1.5f * y1 - 1.5f * y2 + 0.5f * y3;
   float a1 =  y0 - 2.5f * y1 + 2.0f * y2 - 0.5 * y3;
   float a2 = -0.5 * y0 + 0.5f * y2;
   float a3 =  y1;

   return a0 * p * p2 + a1 * p2 + a2 * p + a3;
}

class Point
{
	private:
	float _x = 0.0f, _y = 0.0f, _z = 0.0f;
	public:
	Point(float x, float y, float z)
		: _x(x)
		, _y(y)
		, _z(z)
	{}
	float x (void) const { return _x; }
	float y (void) const { return _y; }
	float z (void) const { return _z; }
	float magnitude (void) const { return sqrt(_x * _x + _y * _y + _z * _z); }
};

Point operator*(const float mul, const Point p)
{
	return Point(mul * p.x(), mul * p.y(), mul * p.z());
}

Point operator+(const Point p1, const Point p2)
{
	return Point(p1.x() + p2.x(), p1.y() + p2.y(), p1.z() + p2.z());
}

Point operator-(const Point p)
{
	return -1.0f * p;
}

Point operator-(const Point p1, const Point p2)
{
	return p1 + -p2;
}


float distance (const Point p1, const Point p2)
{
	return (p1 - p2).magnitude();
}

Point
cubic_interpolate3 (Point p0, Point p1, Point p2, Point p3, float p)
{
	return Point(cubic_interpolate1(p0.x(), p1.x(), p2.x(), p3.x(), p), 
                 cubic_interpolate1(p0.y(), p1.y(), p2.y(), p3.y(), p), 
                 cubic_interpolate1(p0.z(), p1.z(), p2.z(), p3.z(), p));
}

class Curve
{
	private:
	static const size_t segments = 2;
	static const size_t seg_size = 4;	
	static const size_t num_points = seg_size * segments - (segments - 1);
	Point points[num_points] =
	{
		Point(-1.0,  1.0, 0.0), Point(0.0, -1.0, 0.0), 
		Point(0.05, -0.7, 0.0), Point( 0.1, -1.0, 0.0), 
		Point( 0.2, 0.0, 3.0), Point(-0.5, 0.0, 3.0),
		Point(-0.5, 0.2, 3.0)//, Point(0.5, 0.2, 3.0),
	};
	float total_length(size_t end = num_points - 1)
	{
		float ret = 0.0f;
		for(unsigned int i = 0; i < end; ++i)
			ret += distance(points[i], points[i+1]);
		return ret;
	}

	float 
	total_length_cubic(size_t end = num_points - 1)
	{
		// TODO
		assert(end < num_points);
		return total_length(end);
	}

	float 
	total_length_bezier(size_t end = segments)
	{
		// TODO
		assert(end <= segments);
		return total_length(end * seg_size);
	}

	public:
	Point linear_interpolate (float p)
	{
		assert(p <= 1.0f && p >= 0.0f);
		float d = total_length() * p;
		int ii = -1;
		float dd;
		// OPTIMIZE: Could quicksearch
		for (int i = (signed int)num_points - 1; i >= 0; --i)
		{
			if(total_length(i) <= d)
			{
				ii = i;
				dd = d - total_length(i);
				break;
			}
		}
		assert(ii != -1);
		float pp = dd / distance(points[ii], points[ii+1]);
		return points[ii] + pp * (points[ii+1] - points[ii]);
	}


	Point cubic_interpolate (float p)
	{
		assert(p <= 1.0f && p >= 0.0f);
		float d = total_length_cubic() * p;
		int ii = -1;
		float pp0, pp1;
		// OPTIMIZE: Could quicksearch
		for (int i = (signed int)num_points - 1; i >= 0; --i)
		{
			if(total_length_cubic(i) <= d)
			{
				ii = i;
				break;
			}
		}
		assert(ii != -1);
		pp0 = total_length_cubic(ii) / total_length_cubic();
		pp1 = total_length_cubic(ii + 1) / total_length_cubic();
		float pp = (p - pp0) / (pp1 - pp0);

		
		Point p1 = points[ii];
		Point p2 = points[ii  + 1];
		Point p0 = (ii == 0)? p1 : points[ii-1];
		Point p3 = (ii == num_points - 1)? p2 : points[ii+1];

		return cubic_interpolate3(p0, p1, p2, p3, pp);
	}

    Point bezier_approximate(int i, float p)
    {
		assert(p <= 1.0f && p >= 0.0f);
		float con_p = 1 - p;
		float con_p2 = con_p * con_p;
		float p2 = p * p;
        return  con_p2 * con_p * points[0 + (seg_size - 1) * i] +
		        3 * con_p2 * p * points[1 + (seg_size - 1) * i] +
		        3 * con_p * p2 * points[2 + (seg_size - 1) * i] +
		        p * p2         * points[3 + (seg_size - 1) * i];

    }

	Point bezier_approximate (float p)
	{
		assert(p <= 1.0f && p >= 0.0f);
		float d = total_length_bezier() * p;
		int ii = -1;
		float pp0, pp1;
		// OPTIMIZE: Could quicksearch
		for (int i = (signed int)segments; i >= 0; --i)
		{
			if(total_length_bezier(i) <= d)
			{
				ii = i;
				break;
			}
		}
		assert(ii != -1);
		pp0 = total_length_bezier(ii) / total_length_bezier();
		pp1 = total_length_bezier(ii + 1) / total_length_bezier();
		float pp = (p - pp0) / (pp1 - pp0);

		return bezier_approximate(ii, pp);
	}

	void drawPoints(void) const
	{
		glBegin(GL_POINTS);
		for(unsigned int i = 0; i < num_points; ++i)
		{
			glVertex3f(points[i].x(), points[i].y(), points[i].z());
		}
		glEnd();
		glBegin(GL_LINES);
		for(unsigned int i = 0; i < num_points - 1; ++i)
		{
			glVertex3f(points[i].x(), points[i].y(), points[i].z());
			glVertex3f(points[i+1].x(), points[i+1].y(), points[i+1].z());
		}
		glEnd();
	}
};
#endif
