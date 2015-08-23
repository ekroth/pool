/* Copyright (c) 2010-2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#ifndef KKE_COLOR_HPP
#define KKE_COLOR_HPP

typedef unsigned char ubyte;

namespace kke
{
    template<typename T>
    class Color;

    typedef Color<ubyte> Colorb;
    typedef Color<float> Colorf;

    template<typename T>
    class Color
    {
    public:
        constexpr explicit Color(T r = 0, T g = 0, T b = 0, T a = 0) : R(r), G(g), B(b), A(a) { }

        constexpr Color(constexpr Color<T>& color) : R(color.R), G(color.G), B(color.B), A(color.A){ }

        static Colorb GLWhiteb() { return Colorb(255, 255, 255, 255); }

        static Colorf GLWhitef() { return Colorf(1.0f, 1.0f, 1.0f, 1.0f); }
        static Colorb GLFloatToByte(const Colorf &color)
        {
            return Colorb((ubyte)(255.0f * color.R), (ubyte)(255.0f * color.G),
                          (ubyte)(255.0f * color.B), (ubyte)(255.0f * color.A));
        }
        static Colorf GLByteToFloat(const Colorb &color)
        {
            return Colorf(color.R / 255.0f, color.G / 255.0f,
                          color.B / 255.0f, color.A / 255.0f);
        }

        // Operators
        Color<T> operator=(const Color<T>& color)
        {
            R = color.R;
            G = color.G;
            B = color.B;
            A = color.A;

            return *this;
        }

        T R, G, B, A;
    };
}

#endif // KKE_COLOR_HPP