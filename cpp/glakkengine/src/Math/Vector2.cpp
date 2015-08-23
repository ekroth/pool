/* Copyright (c) 2010-2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#include "Math/Vector2.hpp"

#include <math.h>

using kke::Vector2;

template<typename T>
Vector2<T>::Vector2(T x, T y) :
	X(x), Y(y)
{
}

// Static
template<typename T>
Vector2<T> Vector2<T>::Zero()
{
	return Vector2<T> (0, 0);
}

// Static
template<typename T>
Vector2<T> Vector2<T>::One()
{
	return Vector2<T> (1, 1);
}

// Static
template<typename T>
T Vector2<T>::Length(const Vector2<T> &vector)
{
	// The length of a Vector. In 2D the length is the
	// hypothenuse.

	return sqrt(pow(vector.X, 2) + pow(vector.Y, 2));
}

// Static
template<typename T>
Vector2<T> Vector2<T>::Normalize(const Vector2<T> &vector)
{
	// Returns a normalized vector,
	// which results in a unit vector.
	// A unit vector has the length 1

	return vector / Vector2<T>::Length(vector);
}

// Static
template<typename T>
T Vector2<T>::Dot(const Vector2<T> &vec1, const Vector2<T> &vec2)
{
	// The dot product is the "relative orientation" of
	// the vectors. A negative dot product means the vectors
	// are pointing away from each other.

	return (vec1.X * vec2.X) + (vec1.Y * vec2.Y);
}

// Static
template<typename T>
Vector2<T> Vector2<T>::Projection(
	const Vector2<T> &vec1,
	const Vector2<T> &vec2)
{
	// Creates a projection of vec1 on vec2

	// Dot product of the vectors
	const float dotprod = Vector2<T>::Dot(vec1, vec2);

	// The length of vec2 squared
	const float vec2Square = pow(vec2.X, 2.0f) + pow(vec2.Y, 2.0f);

	return Vector2<T> ((dotprod / vec2Square) * vec2.X, (dotprod / vec2Square)
		* vec2.Y);
}

// Static
template<typename T>
Vector2<T> Vector2<T>::RightNormal(const Vector2<T> &vector)
{
	// Right normal of vector. Pointing straight out.

	return Vector2<T> (-vector.Y, vector.X);
}

// Static
template<typename T>
Vector2<T> Vector2<T>::LeftNormal(const Vector2<T> &vector)
{
	// Left normal of vector. Pointing straight out.

	return Vector2<T> (vector.Y, -vector.X);
}

// Static
template<typename T>
T Vector2<T>::Angle(const Vector2<T> &vec1, const Vector2<T> &vec2)
{
	// Angle (not in radians).
	// WARNING: This function is not fully completed.
	return acos(Vector2<T>::Dot(vec1, vec2));
}

// Implementation of static functions
// TODO: Add inline functions to avoid overhead?

template<typename T>
T Vector2<T>::Length() const
{
	return Vector2<T>::Length(*this);
}

template<typename T>
Vector2<T> Vector2<T>::Normalize() const
{
	return Vector2<T>::Normalize(*this);
}

template<typename T>
T Vector2<T>::Dot(const Vector2<T> &vector) const
{
	return Vector2<T>::Dot(*this, vector);
}

template<typename T>
Vector2<T> Vector2<T>::ProjectOn(const Vector2<T> &vector) const
{
	return Vector2<T>::Projection(*this, vector);
}

template<typename T>
Vector2<T> Vector2<T>::Project(const Vector2<T> &vector) const
{
	return Vector2<T>::Projection(vector, *this);
}

template<typename T>
Vector2<T> Vector2<T>::RightNormal() const
{
	return Vector2<T>::RightNormal(*this);
}

template<typename T>
Vector2<T> Vector2<T>::LeftNormal() const
{
	return Vector2<T>::LeftNormal(*this);
}

template<typename T>
T Vector2<T>::Angle(const Vector2<T> &vector) const
{
	return Vector2<T>::Angle(*this, vector);
}

// Operators

template<typename T>
Vector2<T> Vector2<T>::operator=(const Vector2<T> &value)
{
	X = value.X;
	Y = value.Y;

	return *this;
}

template<typename T>
Vector2<T> Vector2<T>::operator+(const Vector2<T> &value) const
{
	return Vector2<T> (X + value.X, Y + value.Y);
}

template<typename T>
Vector2<T> Vector2<T>::operator-(const Vector2<T> &value) const
{
	return Vector2<T> (X - value.X, Y - value.Y);
}

template<typename T>
Vector2<T> Vector2<T>::operator*(T value) const
{
	return Vector2<T> (X * value, Y * value);
}

template<typename T>
Vector2<T> Vector2<T>::operator*(const Vector2<T> &value) const
{
	return Vector2<T> (X * value.X, Y * value.Y);
}

template<typename T>
Vector2<T> Vector2<T>::operator/(const Vector2<T> &value) const
{
	return Vector2<T> (X / value.X, Y / value.Y);
}

template<typename T>
Vector2<T> Vector2<T>::operator/(T value) const
{
	return Vector2<T> (X / value, Y / value);
}

template<typename T>
void Vector2<T>::operator-=(const Vector2<T> &value)
{
	X -= value.X;
	Y -= value.Y;
}

template<typename T>
void Vector2<T>::operator+=(const Vector2<T>& value)
{
	X += value.X;
	Y += value.Y;
}

template<typename T>
void Vector2<T>::operator*=(const Vector2<T> &value)
{
	X *= value.X;
	Y *= value.Y;
}

template<typename T>
void Vector2<T>::operator*=(T value)
{
	X *= value;
	Y *= value;
}

template<typename T>
void Vector2<T>::operator/=(const Vector2<T> &value)
{
	X /= value.X;
	Y /= value.Y;
}

template<typename T>
void Vector2<T>::operator/=(T value)
{
	X /= value;
	Y /= value;
}

template<typename T>
T Vector2<T>::operator[](int index) const
{
	switch (index)
	{
		case 0:
			return X;
			break;

		case 1:
			return Y;
			break;

		default:
			throw "Vector2 operator[] index out of range.";
			break;
	}
}

template<typename T>
bool Vector2<T>::operator==(const Vector2<T> &value) const
{
	return X == value.X && Y == value.Y;
}

template<typename T>
bool Vector2<T>::operator!=(const Vector2<T> &value) const
{
	return X != value.X || Y != value.Y;
}

// Explicit template instantiation
template class Vector2<int> ;
template class Vector2<float> ;
// template class Vector2<double>;
