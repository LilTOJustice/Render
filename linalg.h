#pragma once

#include <cmath>

template<typename T>
struct T_Vec2
{
    T x, y;

    T_Vec2()
        : x{}, y{}
    {}

    T_Vec2(const T &x, const T &y)
        : x{x}, y{y}
    {}
    
    template<typename O>
    T_Vec2(const T_Vec2<O> &other)
    {
        x = other.x;
        y = other.y;
    }

    template<typename O>
    T_Vec2<T>& operator=(const T_Vec2<O> &other)
    {
        x = other.x;
        y = other.y;
        return *this;
    }
    
    template<typename O>
    T_Vec2<T> operator+(const T_Vec2<O> &other) const
    {
        return T_Vec2<T>{T(x+other.x), T(y+other.y)};
    }

    template<typename O>
    T_Vec2<T> operator-(const T_Vec2<O> &other) const
    {
        return T_Vec2<T>{T(x-other.x), T(y-other.y)};
    }

    template<typename O>
    T_Vec2<T> operator*(const T_Vec2<O> &other) const
    {
        return T_Vec2<T>{T(x*other.x), T(y*other.y)};
    }

    template<typename O>
    T_Vec2<long double> operator/(const T_Vec2<O> &other) const
    {
        return T_Vec2<long double>{(long double)x/other.x, (long double)y/other.y};
    }

    T_Vec2<long double> operator*(const long double scalar) const
    {
        return T_Vec2<long double>{(long double)(x*scalar), (long double)(y*scalar)};
    }

    T_Vec2<long double> operator/(const long double scalar) const
    {
        return T_Vec2<long double>{(long double)(x/scalar), (long double)(y/scalar)};
    }

    long double Mag()
    {
        return hypot(x, y);
    }

    T_Vec2<long double> Norm()
    {
        return (*this)/Mag();
    }
};

template<typename T>
struct T_Vec3
{
    T x, y, z;
    T &r, &g, &b;
    T &h, &s, &v;
    const T_Vec2<T&> xy, xz, yz;
    
    T_Vec3()
        : x{}, y{}, z{}
        , r{x}, g{y}, b{z}
        , h{x}, s{y}, v{z}
        , xy{T_Vec2<T&>{x, y}}
        , xz{T_Vec2<T&>{x, z}}
        , yz{T_Vec2<T&>{y, z}}
    {}

    T_Vec3(const T &_x, const T &_y, const T &_z)
        : x{_x}, y{_y}, z{_z}
        , r{x}, g{y}, b{z}
        , h{x}, s{y}, v{z}
        , xy{T_Vec2<T&>{x, y}}
        , xz{T_Vec2<T&>{x, z}}
        , yz{T_Vec2<T&>{y, z}}
    {}

    template<typename O>
    T_Vec3(const T_Vec3<O> &other)
    {
        x = other.x;
        y = other.y;
        z = other.z;
    }

    template<typename O>
    T_Vec3<T>& operator=(const T_Vec3<O> &other)
    {
        x = other.x;
        y = other.y;
        z = other.z;
        return *this;
    }

    template<typename O>
    T_Vec3<T> operator+(const T_Vec3<O> &other) const
    {
        return T_Vec3<T>{T(x+other.x), T(y+other.y), T(z+other.z)};
    }

    template<typename O>
    T_Vec3<T> operator-(const T_Vec3<O> &other) const
    {
        return T_Vec3<T>{T(x-other.x), T(y-other.y), T(z-other.z)};
    }

    template<typename O>
    T_Vec3<T> operator*(const T_Vec3<O> &other) const
    {
        return T_Vec3<T>{T(x*other.x), T(y*other.y), T(z*other.z)};
    }

    template<typename O>
    T_Vec3<long double> operator/(const T_Vec3<O> &other) const
    {
        return T_Vec3<long double>{(long double)x/other.x, (long double)y/other.y, (long double)z/other.z};
    }

    T_Vec3<long double> operator*(const long double scalar) const
    {
        return T_Vec3<long double>{(long double)(x*scalar), (long double)(y*scalar), (long double)(z*scalar)};
    }

    T_Vec3<long double> operator/(const long double scalar) const
    {
        return T_Vec3<long double>{(long double)(x/scalar), (long double)(y/scalar), (long double)(z/scalar)};
    }

    long double Mag()
    {
        return hypot(x, y, z);
    }

    T_Vec3<long double> Norm()
    {
        return (*this)/Mag();
    }
};

template<typename T>
struct T_Vec4
{
    T x, y, z, w;
    T &r, &g, &b, &a;
    const T_Vec2<T&> xy, xz, xw, yz, yw, zw;
    const T_Vec3<T&> xyz, xyw, xzw, yzw;

    T_Vec4()
        : x{}, y{}, z{},  w{}
        , r{x}, g{y}, b{z}, a{w}
        , xy{T_Vec2<T&>{x, y}}
        , xz{T_Vec2<T&>{x, z}}
        , xw{T_Vec2<T&>{x, w}}
        , yz{T_Vec2<T&>{y, z}}
        , yw{T_Vec2<T&>{y, w}}
        , zw{T_Vec2<T&>{z, w}}
        , xyz{T_Vec3<T&>{x, y, z}}
        , xyw{T_Vec3<T&>{x, y, w}}
        , xzw{T_Vec3<T&>{x, z, w}}
        , yzw{T_Vec3<T&>{y, z, w}}
    {}
    
    T_Vec4(const T &_x, const T &_y, const T &_z, const T &_w)
        : x{_x}, y{_y}, z{_z},  w{_w}
        , r{x}, g{y}, b{z}, a{w}
        , xy{T_Vec2<T&>{x, y}}
        , xz{T_Vec2<T&>{x, z}}
        , xw{T_Vec2<T&>{x, w}}
        , yz{T_Vec2<T&>{y, z}}
        , yw{T_Vec2<T&>{y, w}}
        , zw{T_Vec2<T&>{z, w}}
        , xyz{T_Vec3<T&>{x, y, z}}
        , xyw{T_Vec3<T&>{x, y, w}}
        , xzw{T_Vec3<T&>{x, z, w}}
        , yzw{T_Vec3<T&>{y, z, w}}
    {}

    template<typename O>
    T_Vec4(const T_Vec4<O> &other)
    {
        x = other.x;
        y = other.y;
        z = other.z;
        w = other.w;
    }

    template<typename O>
    T_Vec4<T>& operator=(const T_Vec4<O> &other)
    {
        x = other.x;
        y = other.y;
        z = other.z;
        w = other.w;
        return *this;
    }

    template<typename O>
    T_Vec4<T> operator+(const T_Vec4<O> &other) const
    {
        return T_Vec4<T>{T(x+other.x), T(y+other.y), T(z+other.z), T(w+other.w)};
    }

    template<typename O>
    T_Vec4<T> operator-(const T_Vec4<O> &other) const
    {
        return T_Vec4<T>{T(x-other.x), T(y-other.y), T(z-other.z), T(w-other.w)};
    }

    template<typename O>
    T_Vec4<T> operator*(const T_Vec4<O> &other) const
    {
        return T_Vec4<T>{T(x*other.x), T(y*other.y), T(z*other.z), T(w*other.w)};
    }

    template<typename O>
    T_Vec4<long double> operator/(const T_Vec4<O> &other) const
    {
        return T_Vec4<long double>{(long double)x/other.x, (long double)y/other.y, (long double)z/other.z, (long double)w/other.w};
    }

    T_Vec4<long double> operator*(const long double scalar) const
    {
        return T_Vec4<long double>{(long double)(x*scalar), (long double)(y*scalar), (long double)(z*scalar), (long double)(w*scalar)};
    }

    T_Vec4<long double> operator/(const long double scalar) const
    {
        return T_Vec4<long double>{(long double)(x/scalar), (long double)(y/scalar), (long double)(z/scalar), (long double)(w/scalar)};
    }

    long double Mag()
    {
        return hypot(x, y, z, w);
    }

    T_Vec4<long double> Norm()
    {
        return (*this)/Mag();
    }
};

// Math
typedef T_Vec2<long long> Vec2;
typedef T_Vec3<long long> Vec3;
typedef T_Vec4<long long> Vec4;
typedef T_Vec2<long double> fVec2;
typedef T_Vec3<long double> fVec3;
typedef T_Vec4<long double> fVec4;

typedef T_Vec3<long double> Rot;

// Color
typedef T_Vec3<unsigned char> RGB;
typedef T_Vec4<unsigned char> RGBA;
typedef T_Vec3<unsigned char&> RGBRef;
typedef T_Vec4<unsigned char&> RGBARef;
typedef T_Vec3<long double> HSV;

// Color functions
template<typename T>
T max(T first, T second)
{
    return (first > second ? first : second);
}

template<typename T>
T min(T first, T second)
{
    return (first < second ? first : second);
}

const long double DEGPERPI = 180/3.14159265;
HSV ToHSV(const RGB &rgb);
RGB ToRGB(const HSV &hsv);
