#include "matrix33.h"
#include "matrixmath.h"
#include <iostream>
#include <iomanip>




Matrix33::Matrix33()
{
	for (int i=0;i<3;i++)
		for (int j =0;j<3;j++)
				m_m[i][j] = 0.0f;
}

Matrix33::Matrix33(double a11, double a12, double a13, double a21, double a22, double a23, double a31, double a32, double a33)
{
	m_m[0][0] = a11;
	m_m[0][1] = a12;
	m_m[0][2] = a13;
	m_m[1][0] = a21;
	m_m[1][1] = a22;
	m_m[1][2] = a23;
	m_m[2][0] = a31;
	m_m[2][1] = a32;
	m_m[2][2] = a33;
}

Matrix33::Matrix33(Vector3 v1, Vector3 v2, Vector3 v3)
{
	m_m[0][0] = v1.x();
	m_m[0][1] = v1.y();
	m_m[0][2] = v1.z();
	m_m[1][0] = v2.x();
	m_m[1][1] = v2.y();
	m_m[1][2] = v2.z();
	m_m[2][0] = v3.x();
	m_m[2][1] = v3.y();
	m_m[2][2] = v3.z();
}

Matrix33::Matrix33(const double a[])
{
	m_m[0][0] = a[0];
	m_m[0][1] = a[1];
	m_m[0][2] = a[2];
	m_m[1][0] = a[3];
	m_m[1][1] = a[4];
	m_m[1][2] = a[5];
	m_m[2][0] = a[6];
	m_m[2][1] = a[7];
	m_m[2][2] = a[8];
}

Matrix33::Matrix33(const Matrix33& m)
{
	for (int i=0;i<3;i++)
		for (int j =0;j<3;j++)
			m_m[i][j] = m.m_m[i][j];
}

Matrix33::~Matrix33()
{
}

void Matrix33::identity()
{
	for (int i=0;i<3;i++)
		for (int j =0;j<3;j++)
		{
			if (i==j)
				m_m[i][j] = 1.0f;
			else
				m_m[i][j] = 0.0f;
		}
}

void Matrix33::transpose()
{
	double temp;
	temp = m_m[0][1];
	m_m[0][1] = m_m[1][0];
	m_m[1][0] = temp;

	temp = m_m[0][2];
	m_m[0][2] = m_m[2][0];
	m_m[2][0] = temp;

	temp = m_m[1][2];
	m_m[1][2] = m_m[2][1];
	m_m[2][1] = temp;
}

Matrix33 Matrix33::transposed() const
{
	return Matrix33(m_m[0][0], m_m[1][0], m_m[2][0],
				    m_m[0][1], m_m[1][1], m_m[2][1], 
				    m_m[0][2], m_m[1][2], m_m[2][2]);
}

double Matrix33::determinant() const
{
	return m_m[0][0]*m_m[1][1]*m_m[2][2] + m_m[0][1]*m_m[1][2]*m_m[2][0] + m_m[1][0]*m_m[2][1]*m_m[0][2]
		 - m_m[0][2]*m_m[1][1]*m_m[2][0] - m_m[1][0]*m_m[0][2]*m_m[2][2] - m_m[0][0]*m_m[2][1]*m_m[1][2];
}

Matrix33 Matrix33::cofactor()
{
	Matrix33 cofactormatrix;
	double minormatrix[4];
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
		{
			minor(i, j, minormatrix);
			if (0 == (i+j)%2)
				cofactormatrix.m_m[i][j] = determinant2x2(minormatrix);
			else
				cofactormatrix.m_m[i][j] = -determinant2x2(minormatrix);
		}
	return cofactormatrix;
}

void Matrix33::minor(int row, int colom, double theminor[])
{
	int index = 0;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (row != i && colom != j && index < 5)
			{
				theminor[index] = m_m[i][j];
				index++;
			}
			else
            {
				if ( index > 4)
				{

                }
            }
		}
	}
}

double* Matrix33::operator [](size_t i)
{
	if (i > 2)
		return 0;
	return m_m[i];
}

Matrix33 Matrix33::multiply(const Matrix33& matrix)
{
	return Matrix33(
		m_m[0][0]*matrix.m_m[0][0]+m_m[0][1]*matrix.m_m[1][0]+m_m[0][2]*matrix.m_m[2][0],		m_m[0][0]*matrix.m_m[0][1]+m_m[0][1]*matrix.m_m[1][1]+m_m[0][2]*matrix.m_m[2][1],		m_m[0][0]*matrix.m_m[0][2]+m_m[0][1]*matrix.m_m[1][2]+m_m[0][2]*matrix.m_m[2][2],
		m_m[1][0]*matrix.m_m[0][0]+m_m[1][1]*matrix.m_m[1][0]+m_m[1][2]*matrix.m_m[2][0],		m_m[1][0]*matrix.m_m[0][1]+m_m[1][1]*matrix.m_m[1][1]+m_m[1][2]*matrix.m_m[2][1],		m_m[1][0]*matrix.m_m[0][2]+m_m[1][1]*matrix.m_m[1][2]+m_m[1][2]*matrix.m_m[2][2],
		m_m[2][0]*matrix.m_m[0][0]+m_m[2][1]*matrix.m_m[1][0]+m_m[2][2]*matrix.m_m[2][0],		m_m[2][0]*matrix.m_m[0][1]+m_m[2][1]*matrix.m_m[1][1]+m_m[2][2]*matrix.m_m[2][1],		m_m[2][0]*matrix.m_m[0][2]+m_m[2][1]*matrix.m_m[1][2]+m_m[2][2]*matrix.m_m[2][2]);
}

Matrix33 Matrix33::multiply(double scalar)
{
	return Matrix33(
		m_m[0][0]*scalar, m_m[0][1]*scalar, m_m[0][2]*scalar, 
		m_m[1][0]*scalar, m_m[1][1]*scalar, m_m[1][2]*scalar, 
		m_m[2][0]*scalar, m_m[2][1]*scalar, m_m[2][2]*scalar);
}

double Matrix33::determinant2x2(double matrix[4])
{
	return matrix[0]*matrix[3] - matrix[1]*matrix[2];
}

