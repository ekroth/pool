/* Copyright (c) 2010-2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#include "Math/Color.hpp"

using kke::Color;
using kke::Colorb;
using kke::Colorf;

template<typename T>
Color<T>::Color(T r, T g, T b, T a) :
	R(r), G(g), B(b), A(a)
{
}

template<typename T>
Color<T>::Color(const Color<T> &color) :
	R(color.R), G(color.G), B(color.B), A(color.A)
{
}

// Static
template<typename T>
Colorb Color<T>::GLWhiteb()
{
	return Colorb(255, 255, 255, 255);
}

// Static
template<typename T>
Colorf Color<T>::GLWhitef()
{
	return Colorf(1.0f, 1.0f, 1.0f, 1.0f);
}

// Static
template<typename T>
Colorb Color<T>::GLFloatToByte(const Colorf &color)
{
	return Colorb((GLubyte) (255.0f * color.R), (GLubyte) (255.0f * color.G), (GLubyte) (255.0f
		* color.B), (GLubyte) (255.0f * color.A));
}

// Static
template<typename T>
Colorf Color<T>::GLByteToFloat(const Colorb &color)
{
	return Colorf(color.R / 255.0f, color.G / 255.0f, color.B / 255.0f, color.A
		/ 255.0f);
}

// Operators

template<typename T>
Color<T> Color<T>::operator=(const Color<T> &color)
{
	R = color.R;
	G = color.G;
	B = color.B;
	A = color.A;

	return *this;
}

// Explicit template instantiation
template class Color<float> ;
template class Color<unsigned char> ;
