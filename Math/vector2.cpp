#include "Vector2.h"
#include <cmath>

const double Vector2::EQUALPRECISION2 = 0.00001f;



Vector2::Vector2()
{
	m_x = 0.0f;
	m_y = 0.0f;
}

Vector2::Vector2(double x, double y)
{
	m_x = x;
	m_y = y;
}

Vector2::Vector2(const double xy[])
{
	m_x = xy[0];
	m_y = xy[1];
}

void Vector2::normalize()
{
	*this =*this/length();
}

double Vector2::dot(const Vector2& v) const
{
	return (m_x*v.m_x + m_y*v.m_y);
}

double Vector2::length()
{
	return sqrt(dot(*this));
}

void Vector2::operator+= (const Vector2& v)
{
	m_x += v.x();
	m_y += v.y();
}

void Vector2::operator-= (const Vector2& v)
{
	m_x -= v.x();
	m_y -= v.y();
}

void Vector2::operator*= (double scalar)
{
	m_x *= scalar;
	m_y *= scalar;
}

void Vector2::operator/= (double scalar)
{
	m_x /= scalar;
	m_y /= scalar;
}

void Vector2::write(std::ostream &out) const
{
	out << "(" << m_x << ", " << m_y << ")";
}

void Vector2::read(std::istream &in)
{
	char ch;
	in >> ch >> m_x >> ch >> m_y >> ch;
}

Vector2 Vector2::multiply(double scalar) const
{
	return Vector2(m_x * scalar, m_y * scalar);
}

Vector2 Vector2::add(const Vector2& v) const
{
	return Vector2(m_x + v.x(), m_y + v.y());
}

bool Vector2::equal(const Vector2& v) const
{
	if (fabs(m_x - v.x()) < EQUALPRECISION2 && fabs(m_y - v.y()) < EQUALPRECISION2)
		return true;
	return false;
}
