#pragma once

/**
 *simple vec math header
 *
 *made by Defernus for fun
 */

#include <math.h>
#include <iostream>

#define Vec2f Vec<float, 2>
#define Vec3f Vec<float, 3>
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
	
	template<typename ...Args>
	Vec(Args... args)
	{
		static_assert(size > 0, "wrong vector size!");

		static_assert(sizeof...(args) <= size, "to many arguments");
		T arr[sizeof...(args)] = { args... };
		if (sizeof...(args) == size)
		{
			for (T *i = cords, *j = arr; i != cords + size; ++i, ++j)*i = *j;
		}
		else if(sizeof...(args) == 1)
		{
			for (T *i = cords; i != cords + size; ++i)
			{
				*i = arr[0];
			}
		}
	}

	inline T &operator[](const uint32_t &i)
	{
		if (i >= size)throw "wrong cord";
		return *(cords+i);
	}

	T cords[size];
};

template<typename T, size_t size>
inline std::ostream &operator<<(std::ostream &os, const Vec<T, size> &v)
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
	for (T *i = (T*)v.cords, *j = (T*)o.cords; i != (T*)(v.cords + size); ++i, ++j)
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
		d += *i * *j;
	}
	return d;
}

template<typename T>
inline Vec<T, 3> cross(const Vec<T, 3> &l, const Vec<T, 3> &r)
{
	return Vec<T, 3>(l.cords[1]*r.cords[2] - l.cords[2]*r.cords[1], l.cords[2]*r.cords[0] - l.cords[0]*r.cords[2], l.cords[0]*r.cords[1] - l.cords[1]*r.cords[0]);
}


template<typename T, size_t l, size_t n, size_t m>
inline Vec<Vec<T, n>, l> multMat(const Vec<Vec<T, m>, l> &lv, const Vec<Vec<T, n>, m> &rv)
{
	Vec<Vec<T, n>, l> o;

	for (int i = 0; i != l; ++i)
	{
		for (int j = 0; j != n; ++j)
		{
			for (int k = 0; k != m; ++k)
			{
				o.cords[i].cords[j] += lv.cords[i].cords[k] * rv.cords[k].cords[j];
			}
		}
	}
	return o;
}

template<typename T, size_t n, size_t m>
inline Vec<T, n> operator*(const Vec<T, m> &l, const Vec<Vec<T, n>, m> &r)
{
	Vec<T, n> o;

	for (int i = 0; i != n; ++i)
	{
		for (int j = 0; j != m; ++j)
		{
			o.cords[i] += l.cords[j] * r.cords[j].cords[i];
		}
	}

	return o;
}

template<typename T, size_t l, size_t m>
inline Vec<T, l> operator*(const Vec<Vec<T, m>, l> &lv, const Vec<T, m> &rv)
{
	Vec<T, l> o;

	for (int i = 0; i != l; ++i)
	{
		for (int j = 0; j != m; ++j)
		{
			o.cords[i] += lv.cords[i].cords[j] * rv.cords[j];
		}
	}

	return o;
}

template<typename T>
Vec<Vec<T, 4>, 4> getRotationMatrix(Vec<T, 3> v, double a)
{
	double c = std::cos(a);
	double s = std::sin(a);
	return Vec<Vec<T, 4>, 4>
		(
			Vec<T, 4>(c + (1 - c) * v.cords[0] * v.cords[0], (1 - c) * v.cords[0] * v.cords[1] - s * v.cords[2], (1 - c)*v.cords[0] * v.cords[2] + s * v.cords[1], 0),
			Vec<T, 4>((1 - c) * v.cords[1] * v.cords[0] + s * v.cords[2], c + (1 - c) * v.cords[1] * v.cords[1], (1 - c)*v.cords[1] * v.cords[2] - s * v.cords[0], 0),
			Vec<T, 4>((1 - c) * v.cords[2] * v.cords[0] - s * v.cords[1], (1 - c) * v.cords[2] * v[1] + s * v.cords[0], c + (1 - c)*v.cords[2] * v.cords[2], 0),
			Vec<T, 4>(0, 0, 0, 1)
		);
}

template<typename T>
Vec<Vec<T, 4>, 4> getViewMatrix(Vec<T, 3> right, Vec<T, 3> up, Vec<T, 3> front, Vec<T, 3> pos)
{
	return Vec<Vec<T, 4>, 4>
		(
			Vec<T, 4>(right.cords[0], right.cords[1], right.cords[2], -dot(right, pos)),
			Vec<T, 4>(up.cords[0], up.cords[1], up.cords[2], -dot(up, pos)),
			Vec<T, 4>(-front.cords[0], -front.cords[1], front.cords[2], -dot(front, pos)),
			Vec<T, 4>(0, 0, 0, 1)
		);
}

template<typename T>
Vec<Vec<T, 4>, 4> getLookAtMatrix(Vec<T, 3> pos, Vec<T, 3> target, Vec<T, 3> up)
{
	Vec<T, 3> front = normalize(target - pos);
	Vec<T, 3> right = normalize(cross(front, up));
	up = normalize(cross(right, front));

	/*
	return Vec<Vec<T, 4>, 4>
		(
			Vec<T, 4>(right.cords[0], right.cords[1], right.cords[2], -dot(right, pos)),
			Vec<T, 4>(up.cords[0], up.cords[1], up.cords[2], -dot(up, pos)),
			Vec<T, 4>(-front.cords[0], -front.cords[1], -front.cords[2], -dot(front, pos)),
			Vec<T, 4>(0, 0, 0, 1)
		);
		*/
	return Vec<Vec<T, 4>, 4>
		(
			Vec<T, 4>(right.cords[0], up.cords[0], -front.cords[0], 0),
			Vec<T, 4>(right.cords[1], up.cords[1], -front.cords[1], 0),
			Vec<T, 4>(right.cords[2], up.cords[2], -front.cords[2], 0),
			Vec<T, 4>(-dot(right, pos), -dot(up, pos), -dot(front, pos), 1)
		);
}