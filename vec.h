#pragma once

/**
 *simple vec math header
 *
 *made by Defernus for fun
 */

#include <math.h>
#include <iostream>

#define Vecd Vec<double>
#define Vecf Vec<float>
#define Veci Vec<int>

enum Axis
{
	X = 0,
	Y = 1,
	Z = 2
};

template<typename T>
struct Vec
{
	Vec()
	{
		x = 0;
		y = 0;
		z = 0;
	}

	Vec(T x, T y, T z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

	Vec(T a)
	{
		x = a;
		y = a;
		z = a;
	}

	T x, y, z;

	inline T &operator[](const int &i)
	{
		switch (i)
		{
		case Axis::X:
			return x;
		case Axis::Y:
			return y;
		case Axis::Z:
			return z;
		default:
			throw "Vec index is out of range";
		}
	}
};

template<typename T>
inline T mix2d(const T &a, const T &b, const T &c, const double &u, const double &v)
{
	return a * (1 - u - v) + b * u + c * v;
}

template<typename T>
inline T mix(const T &l, const T &r, const double &a)
{
	return l * (1 - a) + r * a;
}

template<typename T>
inline std::ostream &operator<<(std::ostream &os, const Vec<T> &v)
{
	os << v.x << "; " << v.y << "; " << v.z;
	return os;
}

template<typename T>
inline bool operator==(const Vec<T> &l, const Vec<T> &r)
{
	return l.x == r.x && l.y == r.y && l.z == r.z;
}

template<typename T>
inline bool operator!=(const Vec<T> &l, const Vec<T> &r)
{
	return l.x != r.x || l.y != r.y || l.z != r.z;
}

template<typename T>
inline Vec<T> operator+(const Vec<T> &l, const Vec<T> &r)
{
	return Vec<T>(l.x + r.x, l.y + r.y, l.z + r.z);
}

template<typename T>
inline Vec<T> operator-(const Vec<T> &l, const Vec<T> &r)
{
	return Vec<T>(l.x - r.x, l.y - r.y, l.z - r.z);
}

template<typename T>
inline Vec<T> operator*(const Vec<T> &l, const Vec<T> &r)
{
	return Vec<T>(l.x * r.x, l.y * r.y, l.z * r.z);
}

template<typename T, typename T1>
inline Vec<T> operator*(const Vec<T> &v, const T1 &a)
{
	return Vec<T>(T(v.x*a), T(v.y*a), T(v.z*a));
}

template<typename T, typename T1>
inline Vec<T> operator*(const T1 &a, const Vec<T> &v)
{
	return Vec<T>(T(a*v.x), T(a*v.y), T(a*v.z));
}

template<typename T>
inline Vec<T> operator/(const Vec<T> &l, const Vec<T> &r)
{
	return Vec<T>(l.x / r.x, l.y / r.y, l.z / r.z);
}

template<typename T>
inline Vec<T> operator/(const Vec<T> &v, const T &a)
{
	return Vec<T>(v.x / a, v.y / a, v.z / a);
}


template<typename T>
inline Vec<T> operator+=(Vec<T> &l, const Vec<T> &r)
{
	l.x += r.x;
	l.y += r.y;
	l.z += r.z;
	return l;
}

template<typename T>
inline Vec<T> operator-=(Vec<T> &l, const Vec<T> &r)
{
	l.x -= r.x;
	l.y -= r.y;
	l.z -= r.z;
	return l;
}

template<typename T>
inline Vec<T> operator*=(Vec<T> &l, const Vec<T> &r)
{
	l.x *= r.x;
	l.y *= r.y;
	l.z *= r.z;
	return l;
}

template<typename T>
inline Vec<T> operator/=(Vec<T> &l, const Vec<T> &r)
{
	l.x /= r.x;
	l.y /= r.y;
	l.z /= r.z;
	return l;
}

template<typename T>
inline Vec<T> operator*=(Vec<T> &v, const T &a)
{
	v.x *= a;
	v.y *= a;
	v.z *= a;
	return v;
}

template<typename T>
inline Vec<T> operator/=(Vec<T> &v, const T &a)
{
	v.x /= a;
	v.y /= a;
	v.z /= a;
	return v;
}

template<typename T>
inline T sqlen(const Vec<T> &v)
{
	return v.x*v.x + v.y*v.y + v.z*v.z;
}

template<typename T>
inline double len(const Vec<T> &v)
{
	return std::sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
}

template<typename T>
inline Vec<T> normalize(const Vec<T> &v)
{
	T l = std::sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
	return Vec<T>(v.x / l, v.y / l, v.z / l);
}


template<typename T>
inline T dot(const Vec<T> &l, const Vec<T> &r)
{
	return l.x * r.x + l.y * r.y + l.z + r.z;
}

template<typename T>
inline Vec<T> cross(const Vec<T> &l, const Vec<T> &r)
{
	return Vec<T>(l.y*r.z - l.z*r.y, l.z*r.x - l.x*r.z, l.x*r.y - l.y*r.x);
}