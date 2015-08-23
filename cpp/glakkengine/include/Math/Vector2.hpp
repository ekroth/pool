/* Copyright (c) 2010-2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#ifndef KKE_VECTOR2_HPP
#define KKE_VECTOR2_HPP

namespace kke
{
template<typename T>
class Vector2
{
public:
	Vector2(T x = 0, T y = 0);

	static Vector2<T> Zero();
	static Vector2<T> One();

	// Length/Magnitude of vector
	static T Length(const Vector2<T> &vector);
	// Normalize vector
	static Vector2<T> Normalize(const Vector2<T> &vector);
	// Dot product of two vectors
	static T Dot(const Vector2<T> &vec1, const Vector2<T> &vec2);
	// Projects vec1 onto vec2
	static Vector2<T>
		Projection(const Vector2<T> &vec1, const Vector2<T> &vec2);
	// Right normal of vector
	static Vector2<T> RightNormal(const Vector2<T> &vector);
	// Left normal of vector
	static Vector2<T> LeftNormal(const Vector2<T> &vector);
	// Angle between two vectors. WARNING: Not fully implemented!
	static T Angle(const Vector2<T> &vec1, const Vector2<T> &vec2);

	// Implementation of static functions

	// Get length of the vector
	T Length() const;
	// Normalized vector
	Vector2<T> Normalize() const;
	// Dot product of vector
	T Dot(const Vector2<T> &vector) const;
	// The projection of this vector on specified vector
	Vector2<T> ProjectOn(const Vector2<T> &vector) const;
	// The projection of specified vector on this vector
	Vector2<T> Project(const Vector2<T> &vector) const;
	// Right normal
	Vector2<T> RightNormal() const;
	// Left normal
	Vector2<T> LeftNormal() const;
	// Angle between vectors
	T Angle(const Vector2<T> &vector) const;

	// Operators

	Vector2<T> operator=(const Vector2<T> &value);
	Vector2<T> operator+(const Vector2<T> &value) const;
	Vector2<T> operator-(const Vector2<T> &value) const;
	Vector2<T> operator*(T value) const;
	Vector2<T> operator*(const Vector2<T> &value) const;
	Vector2<T> operator/(const Vector2<T> &value) const;
	Vector2<T> operator/(T value) const;

	void operator-=(const Vector2<T> &value);
	void operator+=(const Vector2<T> &value);
	void operator*=(const Vector2<T> &value);
	void operator*=(T value);
	void operator/=(const Vector2<T> &value);
	void operator/=(T value);

	T operator[](int index) const;
	bool operator==(const Vector2<T> &value) const;
	bool operator!=(const Vector2<T> &value) const;

	// Data members
	T X, Y;
};

typedef Vector2<float> Vector2f;
typedef Vector2<int> Vector2i;
}

#endif // KKE_VECTOR2_HPP
