/* Copyright (c) 2010-2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#include "Math/Rect.hpp"

using kke::Rect;

template<typename T>
Rect<T>::Rect(T x, T y, T width, T height) :
	X(x), Y(y), Width(width), Height(height)
{
}

// Static
template<typename T>
Rect<T> Rect<T>::Zero()
{
	return Rect<T> (0, 0, 0, 0);
}

template<typename T>
T Rect<T>::Left() const
{
	return X;
}

template<typename T>
T Rect<T>::Right() const
{
	return X + Width;
}

template<typename T>
T Rect<T>::Top() const
{
	return Y;
}

template<typename T>
T Rect<T>::Bottom() const
{
	return Y + Height;
}

template<typename T>
bool Rect<T>::Contains(const Vector2<T>& point) const
{
	return point.X >= X && point.X <= (X + Width) && point.Y >= Y && point.Y
		<= (Y + Height);
}

template<typename T>
bool Rect<T>::Intersects(const Rect& value) const
{
	return !(this->Left() > value.Right() || this->Right() < value.Left()
		|| this->Bottom() < value.Top() || this->Top() > value.Bottom());
}

template<typename T>
Rect<T> Rect<T>::operator=(const Rect<T>& value)
{
	X = value.X;
	Y = value.Y;
	Width = value.Width;
	Height = value.Height;

	return *this;
}

template<typename T>
bool Rect<T>::operator==(const Rect<T>& value) const
{
	return this->X == value.X && this->Y == value.Y && this->Width
		== value.Width && this->Height == value.Height;
}

template<typename T>
bool Rect<T>::operator!=(const Rect<T>& value) const
{
	return this->X != value.X || this->Y != value.Y || this->Width
		!= value.Width || this->Height != value.Height;
}

// Explicit template instantiation
template class Rect<int>;
template class Rect<float>;
