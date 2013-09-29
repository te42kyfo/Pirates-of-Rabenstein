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

#ifndef RABENSTEIN__GRID_HPP
#define RABENSTEIN__GRID_HPP

#include <cassert>
#include <cstddef>
#include <stdexcept>
#include <iostream>
#include <cstring>
#include <string>

namespace Rabenstein {

template<typename T>
class Grid;
template<typename T>
std::ostream& operator<<(std::ostream &dest, const Grid<T>& grid);
template<typename T>
std::istream& operator>>(std::istream &src, Grid<T>& grid);

template<typename T>
class Grid {
public:
    Grid() : _x(), _y(), _data(nullptr) {}

    Grid(size_t x, size_t y)
        : _x(x), _y(y) {
        _data = new T[_x * _y];
    }

    Grid(const Grid<T>& other)
        : _x(other._x), _y(other._y) {
        _data = new T[_x * _y] ;
        for(size_t iy = 0; iy < _y; ++iy) {
            for(size_t ix = 0; ix < _x; ++ix) {
                (*this)(ix, iy) = other(ix, iy);
            }
        }
    }

    Grid(Grid&& other) noexcept
        : _x(other._x), _y(other._y) {
        _data = other._data;
        other._data = nullptr;
    }

    ~Grid() {
        delete[] _data;
    }

    Grid& operator=(const Grid& other) {
        _x = other._x;
        _y = other._y;
        delete[] _data;
        _data = new T[_x * _y];
        for(size_t iy = 0; iy < _y; ++iy) {
            for(size_t ix = 0; ix < _x; ++ix) {
                (*this)(ix, iy) = other(ix, iy);
            }
        }
        return *this;
    }

    Grid& operator=(Grid&& other) noexcept {
        _x = other._x;
        _y = other._y;
        delete[] _data;
        _data = other._data;
        other._data = nullptr;
        return *this;
    }

    inline T& operator() (size_t x, size_t y) {
        return _data[y * _x + x];
    }

    inline const T& operator() (size_t x, size_t y) const {
        return _data[y * _x + x];
    }

    static void swap(Grid<T>& g1, Grid<T>& g2) {
        assert (g1._x == g2._x);
        assert (g1._y == g2._y);
        T* tmp = g1._data;
        g1._data = g2._data;
        g2._data = tmp;
    }

    inline const T* data() const {
        return _data;
    }

    inline T* data() {
        return _data;
    }

    friend std::ostream& operator<<<>(std::ostream &dest,
                                    const Grid<T>& grid);

    friend std::istream& operator>><>(std::istream &src,
                                    Grid<T>& grid);

    inline size_t x() const { return _x; };
    inline size_t y() const { return _y; };
private:
    size_t _x;
    size_t _y;
    T* _data __attribute__ ((packed));
};

template <typename T>
std::ostream& operator<<(std::ostream &dest, const Grid<T>& grid) {
    dest << grid._x << "\n" << grid._y << "\n";
    for(size_t iy = 0; iy < grid._y; ++iy) {
        for(size_t ix = 0; ix < grid._x; ++ix) {
            dest << grid(ix, iy) << "\n";
        }
    }
    return dest;
}

template <typename T>
std::istream& operator>>(std::istream &src, Grid<T>& grid) {
    delete[] grid._data;
    src >> grid._x;
    src >> grid._y;
    grid._data = new T[grid._x * grid._y];
    for(size_t iy = 0; iy < grid._y; ++iy) {
        for(size_t ix = 0; ix < grid._x; ++ix) {
            src >> grid(ix, iy);
        }
    }
    return src;
}
}
#endif // RABENSTEIN__GRID_HPP
