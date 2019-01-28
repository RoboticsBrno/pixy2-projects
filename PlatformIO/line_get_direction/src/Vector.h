#pragma once

namespace RB {

template<typename T>
struct Vector
{
public:
    Vector() {}

    Vector(T x, T y)
        : _x(x), _y(y)
    {}




private:
    T _x = 0; 
    T _y = 0;
};

} // namespace atoms