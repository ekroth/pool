/* Copyright (c) 2010-2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#ifndef KKE_RECT_HPP
#define KKE_RECT_HPP

#include <glm/glm.hpp>

namespace kke
{
template<typename T>
class Rect
{
public:
    constexpr explicit Rect(T x = 0, T y = 0, T width = 0, T height = 0) : X(x), Y(y), Width(width), Height(height) { }

    explicit Rect(const glm::vec2 &position, T width = 0, T height = 0) : X(position.x), Y(position.y), Width(width), Height(height) { }

    T Left() const { return X; }
    T Right() const { return X; }
    T Top() const { return Y; }
    T Bottom() const { return Y + Height; }

    bool Contains(const glm::vec2& point) const { return point.x >= X && point.x <= (X + Width) && point.y >= Y && point.y <= (Y + Height); }
    bool Intersects(const Rect<T>& value) const { return !(this->Left() > value.Right() || this->Right() < value.Left() || this->Bottom() < value.Top() || this->Top() > value.Bottom()); }

    Rect<T> operator=(const Rect<T> &value)
    {
        X = value.X;
        Y = value.Y;
        Width = value.Width;
        Height = value.Height;

        return *this;
    }

    bool operator==(const Rect<T>& value) const
    {
        return this->X == value.X && this->Y == value.Y && this->Width
               == value.Width && this->Height == value.Height;
    }

    bool operator!=(const Rect<T>& value) const
    {
        return this->X != value.X || this->Y != value.Y || this->Width
               != value.Width || this->Height != value.Height;
    }

    static Rect<T> Zero()
    {
        return Rect<T> (0, 0, 0, 0);
    }

    T X, Y, Width, Height;
};

typedef Rect<int> Recti;
typedef Rect<float> Rectf;
}

#endif // KKE_RECT_HPP