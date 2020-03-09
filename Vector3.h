#pragma once

#include <iostream>
#include "Point3.h"

template <typename T>
inline bool isNaN(const T x) {
	return std::isnan(x);
}
template <>
inline bool isNaN(const int x) {
	return false;
}

template <typename T>
class Point3;

template <typename T>
class Vector3 {
public:

	// Vector3 Public Methods
	T operator[](int i) const {
		assert(i >= 0 && i <= 2);
		if (i == 0) return x;
		if (i == 1) return y;
		return z;
	}
	T& operator[](int i) {
		assert(i >= 0 && i <= 2);
		if (i == 0) return x;
		if (i == 1) return y;
		return z;
	}
	Vector3() { x = y = z = 0; }
	Vector3(T x, T y, T z) : x(x), y(y), z(z) { assert(!HasNaNs()); }
	bool HasNaNs() const { return isNaN(x) || isNaN(y) || isNaN(z); }
	explicit Vector3(const Point3<T>& p);

#ifndef NDEBUG
	// The default versions of these are fine for release builds; for debug
	// we define them so that we can add the Assert checks.
	Vector3(const Vector3<T>& v) {
		assert(!v.HasNaNs());
		x = v.x;
		y = v.y;
		z = v.z;
	}

	Vector3<T>& operator=(const Vector3<T>& v) {
		assert(!v.HasNaNs());
		x = v.x;
		y = v.y;
		z = v.z;
		return *this;
	}
#endif  // !NDEBUG
	Vector3<T> operator+(const Vector3<T>& v) const {
		assert(!v.HasNaNs());
		return Vector3(x + v.x, y + v.y, z + v.z);
	}
	Vector3<T>& operator+=(const Vector3<T>& v) {
		assert(!v.HasNaNs());
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}
	Vector3<T> operator-(const Vector3<T>& v) const {
		assert(!v.HasNaNs());
		return Vector3(x - v.x, y - v.y, z - v.z);
	}
	Vector3<T>& operator-=(const Vector3<T>& v) {
		assert(!v.HasNaNs());
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return *this;
	}
	bool operator==(const Vector3<T>& v) const {
		return x == v.x && y == v.y && z == v.z;
	}
	bool operator!=(const Vector3<T>& v) const {
		return x != v.x || y != v.y || z != v.z;
	}
	template <typename U>
	Vector3<T> operator*(U s) const {
		return Vector3<T>(s * x, s * y, s * z);
	}
	template <typename U>
	Vector3<T>& operator*=(U s) {
		assert(!isNaN(s));
		x *= s;
		y *= s;
		z *= s;
		return *this;
	}
	template <typename U>
	Vector3<T> operator/(U f) const {
		assert(f != 0);
		float inv = (float)1 / f;
		return Vector3<T>(x * inv, y * inv, z * inv);
	}

	template <typename U>
	Vector3<T>& operator/=(U f) {
		assert(f != 0);
		float inv = (float)1 / f;
		x *= inv;
		y *= inv;
		z *= inv;
		return *this;
	}
	Vector3<T> operator-() const { return Vector3<T>(-x, -y, -z); }
	float LengthSquared() const { return x * x + y * y + z * z; }
	float Length() const { return std::sqrt(LengthSquared()); }
	//explicit Vector3(const Normal3<T>& n);

	// Vector3 Public Data
	T x, y, z;
};

template <typename T>
inline std::ostream& operator<<(std::ostream& os, const Vector3<T>& v) {
	os << "[ " << v.x << ", " << v.y << ", " << v.z << " ]";
	return os;
}

typedef Vector3<float> Vector3f;
typedef Vector3<int> Vector3i;
