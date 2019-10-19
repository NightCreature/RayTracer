#include "matrixmath.h"
#include <cmath>

namespace math
{
	double toRadian(double degree)
	{
		return degree * (gmPI / 180);
	}

	double toDegree(double radian)
	{
		return radian * (180 / gmPI);
	}
}
