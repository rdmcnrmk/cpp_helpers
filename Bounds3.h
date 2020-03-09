#pragma once

#include <limits>
#include "Point3.h"

template <typename T>
class Bounds3 {
public:

	// Bounds3 Public Methods
	Bounds3() {
		T minNum = std::numeric_limits<T>::lowest();
		T maxNum = std::numeric_limits<T>::max();
		pMin = Point3<T>(maxNum, maxNum, maxNum);
		pMax = Point3<T>(minNum, minNum, minNum);
	}

	explicit Bounds3(const Point3<T>& p) : pMin(p), pMax(p) {}
	
	Bounds3(const Point3<T>& p1, const Point3<T>& p2)
		: pMin(std::min(p1.x, p2.x), std::min(p1.y, p2.y), std::min(p1.z, p2.z)),
		  pMax(std::max(p1.x, p2.x), std::max(p1.y, p2.y), std::max(p1.z, p2.z)) {}


	const Point3<T>& operator[](int i) const;
	Point3<T>& operator[](int i);

	bool operator==(const Bounds3<T>& b) const {
		return b.pMin == pMin && b.pMax == pMax;
	}
	bool operator!=(const Bounds3<T>& b) const {
		return b.pMin != pMin || b.pMax != pMax;
	}

	Point3<T> Corner(int corner) const {
		assert(corner >= 0 && corner < 8);
		return Point3<T>((*this)[(corner & 1)].x,
			(*this)[(corner & 2) ? 1 : 0].y,
			(*this)[(corner & 4) ? 1 : 0].z);
	}

	Vector3<T> Diagonal() const { return pMax - pMin; }
	
	T SurfaceArea() const {
		Vector3<T> d = Diagonal();
		return 2 * (d.x * d.y + d.x * d.z + d.y * d.z);
	}
	T Volume() const {
		Vector3<T> d = Diagonal();
		return d.x * d.y * d.z;
	}
	int MaximumExtent() const {
		Vector3<T> d = Diagonal();
		if (d.x > d.y && d.x > d.z)
			return 0;
		else if (d.y > d.z)
			return 1;
		else
			return 2;
	}
	Point3<T> Lerp(const Point3f& t) const {
		return Point3<T>(pbrt::Lerp(t.x, pMin.x, pMax.x),
			pbrt::Lerp(t.y, pMin.y, pMax.y),
			pbrt::Lerp(t.z, pMin.z, pMax.z));
	}
	Vector3<T> Offset(const Point3<T>& p) const {
		Vector3<T> o = p - pMin;
		if (pMax.x > pMin.x) o.x /= pMax.x - pMin.x;
		if (pMax.y > pMin.y) o.y /= pMax.y - pMin.y;
		if (pMax.z > pMin.z) o.z /= pMax.z - pMin.z;
		return o;
	}
	void BoundingSphere(Point3<T>* center, float* radius) const {
		*center = (pMin + pMax) / 2;
		*radius = Inside(*center, *this) ? Distance(*center, pMax) : 0;
	}
	template <typename U>
	explicit operator Bounds3<U>() const {
		return Bounds3<U>((Point3<U>)pMin, (Point3<U>)pMax);
	}

	//bool IntersectP(const Ray& ray, float* hitt0 = nullptr, float* hitt1 = nullptr) const;
	//inline bool IntersectP(const Ray& ray, const Vector3f& invDir, const int dirIsNeg[3]) const;
	
	friend std::ostream& operator<<(std::ostream& os, const Bounds3<T>& b) {
		os << "[ " << b.pMin << " - " << b.pMax << " ]";
		return os;
	}

	// Bounds3 Public Data
	Point3<T> pMin, pMax;
};

typedef Bounds3<float> Bounds3f;
typedef Bounds3<int> Bounds3i;

template <typename T>
Bounds3<T> Union(const Bounds3<T>& b, const Point3<T>& p) {
	Bounds3<T> ret;
	ret.pMin = Min(b.pMin, p);
	ret.pMax = Max(b.pMax, p);
	return ret;
}

template <typename T>
Bounds3<T> Union(const Bounds3<T>& b1, const Bounds3<T>& b2) {
	Bounds3<T> ret;
	ret.pMin = Min(b1.pMin, b2.pMin);
	ret.pMax = Max(b1.pMax, b2.pMax);
	return ret;
}

template <typename T>
inline const Point3<T>& Bounds3<T>::operator[](int i) const {
	assert(i == 0 || i == 1);
	return (i == 0) ? pMin : pMax;
}

template <typename T>
inline Point3<T>& Bounds3<T>::operator[](int i) {
	assert(i == 0 || i == 1);
	return (i == 0) ? pMin : pMax;
}