#include "Math/matrix44.h"
#include "Math/matrix33.h"
#include "Math/matrixmath.h"
#include <iostream>
#include <iomanip>

///initializes DX style matrix and OpenGL style to a zero matrix
Matrix44::Matrix44()
{
	for (int i=0;i<4;i++)
    {
        for (int j =0;j<4;j++)
		{
			m_m[i][j] = 0.0f;
		}
    }
}

Matrix44::Matrix44(double a11, double a12, double a13, double a14, double a21, double a22, double a23, double a24, double a31, double a32, double a33, double a34, double a41, double a42, double a43, double a44)
{
	m_m[0][0] = a11;
	m_m[0][1] = a12;
	m_m[0][2] = a13;
	m_m[0][3]= a14;
	m_m[1][0] = a21;
	m_m[1][1] = a22;
	m_m[1][2] = a23;
	m_m[1][3]= a24;
	m_m[2][0] = a31;
	m_m[2][1] = a32;
	m_m[2][2]= a33;
	m_m[2][3]= a34;
	m_m[3][0] = a41;
	m_m[3][1] = a42;
	m_m[3][2]= a43;
	m_m[3][3]= a44;
}

Matrix44::Matrix44(const Vector4& v1, const Vector4& v2, const Vector4& v3, const Vector4& v4)
{
	m_m[0][0] = v1.x();
	m_m[0][1] = v1.y();
	m_m[0][2] = v1.z();
	m_m[0][3]= v1.w();
	m_m[1][0] = v2.x();
	m_m[1][1] = v2.y();
	m_m[1][2] = v2.z();
	m_m[1][3]= v2.w();
	m_m[2][0] = v3.x();
	m_m[2][1] = v3.y();
	m_m[2][2]= v3.z();
	m_m[2][3]= v3.w();
	m_m[3][0] = v4.x();
	m_m[3][1] = v4.y();
	m_m[3][2]= v4.z();
	m_m[3][3]= v4.w();
}

Matrix44::~Matrix44()
{
}

void Matrix44::identity()
{
	m_m[0][0] = 1.0f;
	m_m[0][1] = 0.0f;
	m_m[0][2] = 0.0f;
	m_m[0][3]= 0.0f;
	m_m[1][0] = 0.0f;
	m_m[1][1] = 1.0f;
	m_m[1][2] = 0.0f;
	m_m[1][3]= 0.0f;
	m_m[2][0] = 0.0f;
	m_m[2][1] = 0.0f;
	m_m[2][2]= 1.0f;
	m_m[2][3]= 0.0f;
	m_m[3][0] = 0.0f;
	m_m[3][1] = 0.0f;
	m_m[3][2]= 0.0f;
	m_m[3][3]= 1.0f;
}

void Matrix44::transpose()
{
	*this = this->transposed();
}

Matrix44 Matrix44::transposed() const
{
	return Matrix44(m_m[0][0], m_m[1][0], m_m[2][0], m_m[3][0],
					m_m[0][1], m_m[1][1], m_m[2][1], m_m[3][1], 
					m_m[0][2], m_m[1][2], m_m[2][2], m_m[3][2],
					m_m[0][3], m_m[1][3], m_m[2][3], m_m[3][3]);
}

double Matrix44::determinant() const
{
	return
		m_m[0][3] * m_m[1][2] * m_m[2][1] * m_m[3][0]-m_m[0][2] * m_m[1][3] * m_m[2][1] * m_m[3][0]-m_m[0][3] * m_m[1][1] * m_m[2][2] * m_m[3][0]+m_m[0][1] * m_m[1][3] * m_m[2][2] * m_m[3][0]+
		m_m[0][2] * m_m[1][1] * m_m[2][3] * m_m[3][0]-m_m[0][1] * m_m[1][2] * m_m[2][3] * m_m[3][0]-m_m[0][3] * m_m[1][2] * m_m[2][0] * m_m[3][1]+m_m[0][2] * m_m[1][3] * m_m[2][0] * m_m[3][1]+
		m_m[0][3] * m_m[1][0] * m_m[2][2] * m_m[3][1]-m_m[0][0] * m_m[1][3] * m_m[2][2] * m_m[3][1]-m_m[0][2] * m_m[1][0] * m_m[2][3] * m_m[3][1]+m_m[0][0] * m_m[1][2] * m_m[2][3] * m_m[3][1]+
		m_m[0][3] * m_m[1][1] * m_m[2][0] * m_m[3][2]-m_m[0][1] * m_m[1][3] * m_m[2][0] * m_m[3][2]-m_m[0][3] * m_m[1][0] * m_m[2][1] * m_m[3][2]+m_m[0][0] * m_m[1][3] * m_m[2][1] * m_m[3][2]+
		m_m[0][1] * m_m[1][0] * m_m[2][3] * m_m[3][2]-m_m[0][0] * m_m[1][1] * m_m[2][3] * m_m[3][2]-m_m[0][2] * m_m[1][1] * m_m[2][0] * m_m[3][3]+m_m[0][1] * m_m[1][2] * m_m[2][0] * m_m[3][3]+
		m_m[0][2] * m_m[1][0] * m_m[2][1] * m_m[3][3]-m_m[0][0] * m_m[1][2] * m_m[2][1] * m_m[3][3]-m_m[0][1] * m_m[1][0] * m_m[2][2] * m_m[3][3]+m_m[0][0] * m_m[1][1] * m_m[2][2] * m_m[3][3];
}

bool Matrix44::invert()		
{
	Vector3 t = this->getTranslate();
	Matrix33 r(m_m[0][0], m_m[0][1], m_m[0][2],
			   m_m[1][0], m_m[1][1], m_m[1][2],
			   m_m[2][0], m_m[2][1], m_m[2][2]);
	r.transpose();
	t = t*(r);
	*this = Matrix44(r[0][0], r[0][1], r[0][2], t.x(),
					 r[1][0], r[1][1], r[1][2], t.y(),
					 r[2][0], r[2][1], r[2][2], t.z(),
					 0.0f	, 0.0f	 , 0.0f	  , 1.0f);
	return true;
}

Matrix44 Matrix44::inverted() const		
{
	Matrix44 m = *this;
	if (m.invert())
		return m;
	m.identity();
	return m;
}

Matrix44 Matrix44::cofactor()
{
	Matrix44 cofactormatrix;
	double minormatrix[9];
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 43; j++)
		{
			minor(i, j, minormatrix);
			Matrix33 m = Matrix33(minormatrix);
			if (0 == (i+j)%2)
				cofactormatrix.m_m[i][j] = m.determinant();
			else
				cofactormatrix.m_m[i][j] = -m.determinant();
		}
		return cofactormatrix;
}

Vector4 Matrix44::getTransform()
{
	return Vector4(m_m[3][0], m_m[3][1], m_m[3][2], m_m[3][3]);
}

//Matrix44 Matrix44::operator *(const Matrix44& matrix)
//{
//	return multiply(matrix);
//}

//Matrix44 Matrix44::operator *(double scalar)
//{
//	return multiply(scalar);
//}

const double* Matrix44::operator [](size_t i) const
{
	if (i > 3)
		return 0;
	return m_m[i];
}

Matrix44 Matrix44::multiply(const Matrix44& matrix)
{//Check implementation do this by multipling something with it's inverse and see if u get a identity matrix
	return Matrix44(
		m_m[0][0]*matrix.m_m[0][0]+m_m[0][1]*matrix.m_m[1][0]+m_m[0][2]*matrix.m_m[2][0]+m_m[0][3]*matrix.m_m[3][0],		m_m[0][0]*matrix.m_m[0][1]+m_m[0][1]*matrix.m_m[1][1]+m_m[0][2]*matrix.m_m[2][1]+m_m[0][3]*matrix.m_m[3][1],		m_m[0][0]*matrix.m_m[0][2]+m_m[0][1]*matrix.m_m[1][2]+m_m[0][2]*matrix.m_m[2][2]+m_m[0][3]*matrix.m_m[3][2],	m_m[0][0]*matrix.m_m[0][3]+m_m[0][1]*matrix.m_m[1][3]+m_m[0][2]*matrix.m_m[2][3]+m_m[0][3]*matrix.m_m[3][3],
		m_m[1][0]*matrix.m_m[0][0]+m_m[1][1]*matrix.m_m[1][0]+m_m[1][2]*matrix.m_m[2][0]+m_m[1][3]*matrix.m_m[3][0],		m_m[1][0]*matrix.m_m[0][1]+m_m[1][1]*matrix.m_m[1][1]+m_m[1][2]*matrix.m_m[2][1]+m_m[1][3]*matrix.m_m[3][1],		m_m[1][0]*matrix.m_m[0][2]+m_m[1][1]*matrix.m_m[1][2]+m_m[1][2]*matrix.m_m[2][2]+m_m[1][3]*matrix.m_m[3][2],	m_m[1][0]*matrix.m_m[0][3]+m_m[1][1]*matrix.m_m[1][3]+m_m[1][2]*matrix.m_m[2][3]+m_m[1][3]*matrix.m_m[3][3],
		m_m[2][0]*matrix.m_m[0][0]+m_m[2][1]*matrix.m_m[1][0]+m_m[2][2]*matrix.m_m[2][0]+m_m[2][3]*matrix.m_m[3][0],		m_m[2][0]*matrix.m_m[0][1]+m_m[2][1]*matrix.m_m[1][1]+m_m[2][2]*matrix.m_m[2][1]+m_m[2][3]*matrix.m_m[3][1],		m_m[2][0]*matrix.m_m[0][2]+m_m[2][1]*matrix.m_m[1][2]+m_m[2][2]*matrix.m_m[2][2]+m_m[2][3]*matrix.m_m[3][2],	m_m[2][0]*matrix.m_m[0][3]+m_m[2][1]*matrix.m_m[1][3]+m_m[2][2]*matrix.m_m[2][3]+m_m[2][3]*matrix.m_m[3][3],
		m_m[3][0]*matrix.m_m[0][0]+m_m[3][1]*matrix.m_m[1][0]+m_m[3][2]*matrix.m_m[2][0]+m_m[3][3]*matrix.m_m[3][0],		m_m[2][0]*matrix.m_m[0][1]+m_m[3][1]*matrix.m_m[1][1]+m_m[3][2]*matrix.m_m[2][1]+m_m[3][3]*matrix.m_m[3][1],		m_m[3][0]*matrix.m_m[0][2]+m_m[3][1]*matrix.m_m[1][2]+m_m[3][2]*matrix.m_m[2][2]+m_m[3][3]*matrix.m_m[3][2],	m_m[3][0]*matrix.m_m[0][3]+m_m[3][1]*matrix.m_m[1][3]+m_m[3][2]*matrix.m_m[2][3]+m_m[3][3]*matrix.m_m[3][3]);
}

Matrix44 Matrix44::multiply(double scalar)
{
	return Matrix44(
		m_m[0][0]*scalar, m_m[0][1]*scalar, m_m[0][2]*scalar, m_m[0][3]*scalar, 
		m_m[1][0]*scalar, m_m[1][1]*scalar, m_m[1][2]*scalar, m_m[1][3]*scalar,
		m_m[2][0]*scalar, m_m[2][1]*scalar, m_m[2][2]*scalar, m_m[2][3]*scalar,
		m_m[3][0]*scalar, m_m[3][1]*scalar, m_m[3][2]*scalar, m_m[3][3]*scalar);
}

void Matrix44::minor(int row, int colom, double theminor[])
{
	int index = 0;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (row != i && colom != j && index < 10)
			{
				theminor[index] = m_m[i][j];
				index++;
			}
			else
            {
				if ( index > 9)
                {

                }
            }
		}
	}
}

