#pragma once

/**
 *simple vec math header
 *
 *made by Defernus for fun
 */

#include <math.h>
#include <iostream>

#define Vec3f Vec<float, 3>
#define Vec2f Vec<float, 2>
#define Vec4f Vec<float, 4>

#define Mat2x2f Vec<Vec<float, 2>, 2>
#define Mat3x3f Vec<Vec<float, 3>, 3>
#define Mat4x4f Vec<Vec<float, 4>, 4>

enum Axis
{
	X = 0,
	Y = 1,
	Z = 2,
	W = 3
};


template<typename T, size_t size>
struct Vec
{
	Vec()
	{
		static_assert(size > 0, "wrong vector size!");

		for (T *i = cords; i != cords + size; ++i)
		{
			*i = T();
		}
	}
	
	Vec(T a)
	{
		static_assert(size > 0, "wrong vector size!");
		for (T *i = cords; i != cords + size; ++i)*i = a;
	}
	
	template<typename ...Args>
	Vec(Args... args)
	{
		static_assert(sizeof...(args) == size && sizeof...(args) > 1, "wrong vector size!");
		T arr[sizeof...(args)] = { args... };


		for (T *i = cords, *j = arr; i != cords + size; ++i, ++j)*i = *j;
	}

	inline T &operator[](const uint32_t &i)
	{
		if (i >= size)throw "wrong cord";
		return *(cords+i);
	}

	T cords[size];
};

template<typename T, size_t size>
inline std::ostream &operator<<(std::ostream &os, Vec<T, size> &v)
{
	for (uint32_t i = 0; i != size; ++i)
	{
 		std::cout << *(v.cords+i);
		if (i != size - 1)std::cout << " ";
	}
	return os;
}

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

template<typename T, size_t size>
inline bool operator==(const Vec<T, size> &l, const Vec<T, size> &r)
{
	for (T *i = (T*)l.cords, *j = (T*)r.cords; i != (T*)(l.cords + size); ++i, ++j)
	{
		if (*i != *j)return false;
	}
	return true;
}

template<typename T, size_t size>
inline bool operator!=(const Vec<T, size> &l, const Vec<T, size> &r)
{
	for (T *i = (T*)l.cords, *j = (T*)r.cords; i != (T*)(l.cords + size); ++i, ++j)
	{
		if (*i != *j)return true;
	}
	return false;
}

template<typename T, size_t size>
inline Vec<T, size> operator+(const Vec<T, size> &l, const Vec<T, size> &r)
{
	Vec<T, size> o;
	for (T *i = (T*)l.cords, *j = (T*)r.cords, *k = (T*)o.cords; i != (T*)(l.cords + size); ++i, ++j, ++k)
	{
		*k = *i + *j;
	}
	return o;
}

template<typename T, size_t size>
inline Vec<T, size> operator-(const Vec<T, size> &l, const Vec<T, size> &r)
{
	Vec<T, size> o;
	for (T *i = (T*)l.cords, *j = (T*)r.cords, *k = (T*)o.cords; i != (T*)(l.cords + size); ++i, ++j, ++k)
	{
		*k = *i - *j;
	}
	return o;
}

template<typename T, size_t size>
inline Vec<T, size> operator*(const Vec<T, size> &l, const Vec<T, size> &r)
{
	Vec<T, size> o;
	for (T *i = (T*)l.cords, *j = (T*)r.cords, *k = (T*)o.cords; i != (T*)(l.cords + size); ++i, ++j, ++k)
	{
		*k = *i * *j;
	}
	return o;
}

template<typename T, size_t size, typename T1>
inline Vec<T, size> operator*(const Vec<T, size> &v, const T1 &a)
{
	Vec<T, size> o;
	for (T *i = (T*)v.cords, *j = (T*)o.cords; i != (T*)(v.cords + size); ++i, ++j)
	{
		*j = *i * a;
	}

	return o;
}

template<typename T, size_t size, typename T1>
inline Vec<T, size> operator*(const T1 &a, const Vec<T, size> &v)
{
	Vec<T, size> o;
	for (T *i = (T*)v.cords, *j = (T*)o.cords; i != (T*)(v.cords + size); ++i, ++j)
	{
		*j = a * *i;
	}
	return o;
}

template<typename T, size_t size>
inline Vec<T, size> operator/(const Vec<T, size> &l, const Vec<T, size> &r)
{
	Vec<T, size> o;
	for (T *i = (T*)l.cords, *j = (T*)r.cords, *k = (T*)o.cords; i != (T*)(l.cords + size); ++i, ++j, ++k)
	{
		*k = *i / *j;
	}
	return o;
}

template<typename T, size_t size>
inline Vec<T, size> operator/(const Vec<T, size> &v, const T &a)
{
	Vec<T, size> o;
	for (T *i = (T*)v.cords, *j = (T*)o.cords; i != (T*)(v.cords + size); ++i, ++j)
	{
		*j = *i * a;
	}
	return o;
}

template<typename T, size_t size>
inline Vec<T, size> operator/(const T &a, const Vec<T, size> &v)
{
	Vec<T, size> o;
	for (T *i = (T*)v.cords, *j = (T*)o.cords; i != (T*)(v.cords + size); ++i, ++j)
	{
		*j = a/ *i;
	}
	return o;
}


template<typename T, size_t size>
inline Vec<T, size> operator+=(Vec<T, size> &l, const Vec<T, size> &r)
{
	Vec<T, size> o;
	for (T *i = (T*)l.cords, *j = (T*)r.cords; i != (T*)(l.cords + size); ++i, ++j)
	{
		*i += *j;
	}
	return l;
}

template<typename T, size_t size>
inline Vec<T, size> operator-=(Vec<T, size> &l, const Vec<T, size> &r)
{
	Vec<T, size> o;
	for (T *i = (T*)l.cords, *j = (T*)r.cords; i != (T*)(l.cords + size); ++i, ++j)
	{
		*i -= *j;
	}
	return l;
}

template<typename T, size_t size>
inline Vec<T, size> operator*=(Vec<T, size> &l, const Vec<T, size> &r)
{
	Vec<T, size> o;
	for (T *i = (T*)l.cords, *j = (T*)r.cords; i != (T*)(l.cords + size); ++i, ++j)
	{
		*i *= *j;
	}
	return l;
}

template<typename T, size_t size>
inline Vec<T, size> operator/=(Vec<T, size> &l, const Vec<T, size> &r)
{
	Vec<T, size> o;
	for (T *i = (T*)l.cords, *j = (T*)r.cords; i != (T*)(l.cords + size); ++i, ++j)
	{
		*i /= *j;
	}
	return l;
}

template<typename T, size_t size>
inline Vec<T, size> operator*=(Vec<T, size> &v, const T &a)
{
	for (T *i = (T*)v.cords; i != (T*)(v.cords + size); ++i)
	{
		*i *= a;
	}
	return v;
}

template<typename T, size_t size>
inline Vec<T, size> operator/=(Vec<T, size> &v, const T &a)
{
	for (T *i = (T*)v.cords; i != (T*)(v.cords + size); ++i)
	{
		*i /= a;
	}
	return v;
}

template<typename T, size_t size>
inline T sqlen(const Vec<T, size> &v)
{
	T l = T();
	for (T *i = (T*)v.cords; i != (T*)(v.cords + size); ++i)
	{
		l+=*i * *i;
	}
	return l;
}

template<typename T, size_t size>
inline double len(const Vec<T, size> &v)
{
	return std::sqrt(sqlen(v));
}

template<typename T, size_t size>
inline Vec<T, size> normalize(const Vec<T, size> &v)
{
	T l = len(v);

	Vec<T, size> o;
	for (T *i = (T*)l.x, *j = (T*)o.cords; i != (T*)(l.x + size); ++i, ++j)
	{
		*j = *i / l;
	}
	return o;
}


template<typename T, size_t size>
inline T dot(const Vec<T, size> &l, const Vec<T, size> &r)
{
	T d = T();
	for (T *i = (T*)l.cords, *j = (T*)r.cords; i != (T*)(l.cords + size); ++i, ++j)
	{
		l += *i * *j;
	}
	return d;
}

template<typename T, size_t size>
inline Vec<T, size> cross(const Vec<T, size> &l, const Vec<T, size> &r)
{
	return Vec<T>(l.y*r.z - l.z*r.y, l.z*r.cords - l.cords*r.z, l.cords*r.y - l.y*r.cords);
}