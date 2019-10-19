#ifndef VECTOR2_H
#define VECTOR2_H

#include <iostream>

class Vector2
{
public:
	Vector2();
	Vector2(double x, double y);
	Vector2(const double xy[]);
	~Vector2() {}

	double dot (const Vector2& v) const;
	double length();
	void normalize();
	Vector2 multiply (double scalar) const;
	Vector2 add(const Vector2& v) const;
	bool equal(const Vector2& v) const;

	void operator+= (const Vector2& v);
	void operator-= (const Vector2& v);
	void operator*= (double scalar);
	void operator/= (double scalar);

	double x() const {return m_x; }
	double y() const {return m_y; }

	void read(std::istream& in);
	void write(std::ostream& out) const;

	static Vector2 zero() {return Vector2(0.0f, 0.0f);}
	static Vector2 xAxis() {return Vector2(1.0f, 0.0f);}
	static Vector2 yAxis() {return Vector2(0.0f, 1.0f);}

protected:
private:
	double m_x, m_y;
	static const double EQUALPRECISION2;
};

inline Vector2 operator- (const Vector2& v)
{
	return Vector2(-v.x(), -v.y());
}

inline Vector2 operator+ (const Vector2& lhs, const Vector2& rhs)
{
	return lhs.add(rhs);
}

inline Vector2 operator- (const Vector2& lhs, const Vector2& rhs)
{
	return lhs.add(rhs);
}

inline Vector2 operator* (const Vector2& lhs, double rhs)
{
	return lhs.multiply(rhs);
}

inline Vector2 operator* (double lhs, const Vector2& rhs)
{
	return rhs.multiply(lhs);
}

inline Vector2 operator/ (const Vector2& lhs, double rhs)
{
	return lhs.multiply(1/rhs);
}

inline Vector2 operator/ (double lhs, const Vector2& rhs)
{
	return rhs.multiply(1/lhs);
}

///Determines the minimum of two vectors used in BBoxes
inline Vector2 minv (const Vector2& a, const Vector2& b)
{
	return Vector2(
		a.x() < b.x() ? a.x() : b.x(),
		a.y() < b.y() ? a.y() : b.y());
}

///Determines the maximum of two vectors used in BBoxes
inline Vector2 maxv (const Vector2& a, const Vector2& b)
{
	return Vector2(
		a.x() > b.x() ? a.x() : b.x(),
		a.y() > b.y() ? a.y() : b.y());
}

inline std::ostream& operator<< (std::ostream& out, const Vector2& v)
{
	v.write(out);
	return out;
}

inline std::istream& operator>> (std::istream& in, Vector2& v)
{
	v.read(in);
	return in;
}

inline bool operator== (const Vector2& lhs, const Vector2& rhs)
{
	return lhs.equal(rhs);
}
#endif