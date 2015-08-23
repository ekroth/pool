/* Copyright (c) 2010-2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#include "Math/Vector3.hpp"

using kke::Vector3;

template<typename T>
Vector3<T>::Vector3(T x, T y, T z) :
	X(x), Y(y), Z(z)
{
}

// Static
template<typename T>
Vector3<T> Vector3<T>::Zero()
{
	return Vector3<T> (0, 0, 0);
}

// Static
template<typename T>
Vector3<T> Vector3<T>::One()
{
	return Vector3<T> (1, 1, 1);
}

template<typename T>
Vector3<T> Vector3<T>::operator=(const Vector3<T> &value)
{
	X = value.X;
	Y = value.Y;
	Z = value.Z;

	return *this;
}

template<typename T>
Vector3<T> Vector3<T>::operator+(const Vector3<T> &value) const
{
	return Vector3<T> (X + value.X, Y + value.Y, Z + value.Z);
}

template<typename T>
Vector3<T> Vector3<T>::operator-(const Vector3<T> &value) const
{
	return Vector3<T> (X - value.X, Y - value.Y, Z - value.Z);
}

template<typename T>
Vector3<T> Vector3<T>::operator*(T value) const
{
	return Vector3<T> (X * value, Y * value, Z * value);
}

template<typename T>
Vector3<T> Vector3<T>::operator*(const Vector3<T> &value) const
{
	return Vector3<T> (X * value.X, Y * value.Y, Z * value.Z);
}

template<typename T>
Vector3<T> Vector3<T>::operator/(const Vector3<T> &value) const
{
	return Vector3<T> (X / value.X, Y / value.Y, Z / value.Z);
}

template<typename T>
Vector3<T> Vector3<T>::operator/(T value) const
{
	return Vector3<T> (X / value, Y / value, Z / value);
}

template<typename T>
void Vector3<T>::operator-=(const Vector3<T> &value)
{
	X -= value.X;
	Y -= value.Y;
	Z -= value.Z;
}

template<typename T>
void Vector3<T>::operator+=(const Vector3<T> &value)
{
	X += value.X;
	Y += value.Y;
	Z += value.Z;
}

template<typename T>
void Vector3<T>::operator*=(const Vector3<T> &value)
{
	X *= value.X;
	Y *= value.Y;
	Z *= value.Z;
}

template<typename T>
void Vector3<T>::operator*=(T value)
{
	X *= value;
	Y *= value;
	Z *= value;
}

template<typename T>
void Vector3<T>::operator/=(const Vector3<T> &value)
{
	X /= value.X;
	Y /= value.Y;
	Z /= value.Z;
}

template<typename T>
void Vector3<T>::operator/=(T value)
{
	X /= value;
	Y /= value;
	Z /= value;
}

template<typename T>
T Vector3<T>::operator[](int index) const
{
	switch (index)
	{
		case 0:
			return X;
			break;

		case 1:
			return Y;
			break;

		case 2:
			return Z;
			break;

		default:
			throw "Vector3 operator[] index out of range.";
			break;
	}
}

template<typename T>
bool Vector3<T>::operator==(const Vector3<T> &value) const
{
	return X == value.X && Y == value.Y && Z == value.Z;
}

template<typename T>
bool Vector3<T>::operator!=(const Vector3<T> &value) const
{
	return X != value.X || Y != value.Y || Z != value.Z;
}

// Explicit template instantiation
template class Vector3<float> ;
template class Vector3<int> ;
