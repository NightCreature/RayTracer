#ifndef MATRIXMATH_H
#define MATRIXMATH_H

#include "vector2.h"
#include "vector3.h"
#include "vector4.h"
#include "matrix33.h"
#include "matrix44.h"

#include <cmath>

namespace math
{
	const double gmPI = 3.14159265;
	double toRadian(double degree);
	double toDegree(double radian);
	template <class T>
	T lerp(const T &a, const T &b, const double pos) 
	{
		return pos * a + (1 - pos) * b;
	}

    inline Matrix44 createLeftHandedFOVPerspectiveMatrix(double fieldOfViewY, double aspectRatio, double znearPlane, double zfarPlane)
    {
        double height = 1.0 / tan(fieldOfViewY/2);
        double width =  height / aspectRatio;
        return Matrix44(width, 0.0, 0.0, 0.0,
                        0.0,  height, 0.0, 0.0,
                        0.0, 0.0, zfarPlane/(zfarPlane-znearPlane), 1.0,
                        0.0, 0.0, -znearPlane*zfarPlane/(zfarPlane-znearPlane), 0.0);
    }

	inline Matrix44 createOrthoGraphicProjection(double width, double height, double zNear, double zFar)
	{
		return Matrix44(
			2 / width, 0,		   0,					   0,
			0,		   2 / height, 0,				       0,
			0,         0,          1 / (zFar - zNear),	   0,
			0,	       0,          zNear / (zNear - zFar), 1
			);
	}
}
#endif