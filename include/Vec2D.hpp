/* Copyright (C) 2013  Marco Heisig

This file is part of Feldrand.

Feldrand is free software: you can redistribute it and/or modify it under the
terms of the GNU Affero General Public License as published by the Free
Software Foundation, either version 3 of the License, or (at your option) any
later version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for more
details.

You should have received a copy of the GNU Affero General Public License along
with this program.  If not, see <http://www.gnu.org/licenses/>. */

#ifndef RABENSTEIN__VECTOR_HPP
#define RABENSTEIN__VECTOR_HPP

#include <cmath>

namespace Rabenstein {

template<typename T>
class Vec2D;
template<typename T>
std::ostream& operator<<(std::ostream &dest, const Vec2D<T>& vec);
template<typename T>
std::istream& operator>>(std::istream &src, Vec2D<T>& vec);

template<typename T>
class Vec2D {
public:
    T x;
    T y;
    Vec2D() : x(), y() {};
    Vec2D(T x, T y) : x(x), y(y) {};
    Vec2D(const Vec2D& other)
        : x(other.x), y(other.y){}
    Vec2D& operator=(const Vec2D& other) {
        x = other.x;
        y = other.y;
        return *this;
    }
    Vec2D& operator+=(const Vec2D& other) {
        x += other.x;
        y += other.y;
        return *this;
    }
    Vec2D& operator-=(const Vec2D& other) {
        x -= other.x;
        y -= other.y;
        return *this;
    }
    Vec2D& operator*=(const T& scalar) {
        x *= scalar;
        y *= scalar;
        return *this;
    }
    Vec2D& operator/=(const T& scalar) {
        x /= scalar;
        y /= scalar;
        return *this;
    }
    Vec2D operator*(const T& scalar) const {
        Vec2D tmp = *this;
        tmp *= scalar;
        return tmp;
    }
    Vec2D operator/(const T& scalar) const {
        Vec2D tmp = *this;
        tmp /= scalar;
        return tmp;
    }
    Vec2D operator+(const Vec2D& other) const {
        Vec2D tmp = *this;
        tmp += other;
        return tmp;
    }
    Vec2D operator-(const Vec2D& other) const {
        Vec2D tmp = *this;
        tmp -= other;
        return tmp;
    }
    T operator*(const Vec2D& other) const {
        return x * other.x + y * other.y;
    }
    T abs() const {
        return std::sqrt((*this) * (*this));
    }
    T sqlength() const {
        return x*x+y*x;
    }
    Vec2D& normalize() {
        T n = (*this).abs();
        // avoid SIGFPE
        if(n != 0.0) n = 1.0 / n;
        (*this) *= n;
        return *this;
    }

    friend std::ostream& operator<<<>(std::ostream &dest,
                                      const Vec2D<T>& vec);
    friend std::istream& operator>><>(std::istream &src,
                                      Vec2D<T>& vec);
} __attribute__ ((packed));

template <typename T>
std::ostream& operator<<(std::ostream &dest,
                                const Vec2D<T>& vec) {
    dest << vec.x << " " << vec.y << "\n";
    return dest;
}

template <typename T>
std::istream& operator>>(std::istream &src,
                                  Vec2D<T>& vec) {
    src >> vec.x >> vec.y;
    return src;
}
}
#endif // RABENSTEIN__VECTOR_HPP
