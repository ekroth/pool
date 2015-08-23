/* Copyright (c) 2010-2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#ifndef KKE_VECTOR3_HPP
#define KKE_VECTOR3_HPP

namespace kke
{
template<typename T>
class Vector3
{
public:
	Vector3(T x = 0, T y = 0, T z = 0);

	static Vector3<T> Zero();
	static Vector3<T> One();

	Vector3<T> operator=(const Vector3<T> &value);
	Vector3<T> operator+(const Vector3<T> &value) const;
	Vector3<T> operator-(const Vector3<T> &value) const;
	Vector3<T> operator*(T value) const;
	Vector3<T> operator*(const Vector3<T> &value) const;
	Vector3<T> operator/(const Vector3<T> &value) const;
	Vector3<T> operator/(T value) const;

	void operator-=(const Vector3<T> &value);
	void operator+=(const Vector3<T> &value);
	void operator*=(const Vector3<T> &value);
	void operator*=(T value);
	void operator/=(const Vector3<T> &value);
	void operator/=(T value);

	T operator[](int index) const;
	bool operator==(const Vector3<T> &value) const;
	bool operator!=(const Vector3<T> &value) const;

	T X, Y, Z;
};

typedef Vector3<float> Vector3f;
typedef Vector3<int> Vector3i;
}

#endif // KKE_VECTOR3_HPP
