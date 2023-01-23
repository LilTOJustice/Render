#pragma once

#include <cmath>

typedef unsigned long long ull_t;
typedef long long ll_t;
typedef long double ld_t;
typedef unsigned char byte_t;

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
        : x{T(other.x)}
        , y{T(other.y)}
    {}

    template<typename O>
    T_Vec2<T>& operator=(const T_Vec2<O> &other)
    {
        x = T(other.x);
        y = T(other.y);
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
    T_Vec2<ld_t> operator/(const T_Vec2<O> &other) const
    {
        return T_Vec2<ld_t>{(ld_t)x/other.x, (ld_t)y/other.y};
    }

    T_Vec2<ld_t> operator*(const ld_t scalar) const
    {
        return T_Vec2<ld_t>{(ld_t)(x*scalar), (ld_t)(y*scalar)};
    }

    T_Vec2<ld_t> operator/(const ld_t scalar) const
    {
        return T_Vec2<ld_t>{(ld_t)(x/scalar), (ld_t)(y/scalar)};
    }

    template<typename O>
    bool operator==(const T_Vec2<O> &other) const
    {
        return x == other.x && y == other.y;
    }

    ld_t Mag()
    {
        return hypot(x, y);
    }

    T_Vec2<ld_t> Norm()
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
        : x{T(other.x)}
        , y{T(other.y)}
        , z{T(other.z)}
        , r{x}
        , g{y}
        , b{z}
        , h{x}
        , s{y}
        , v{z}
        , xy{T_Vec2<O&>{x, y}}
        , xz{T_Vec2<O&>{x, z}}
        , yz{T_Vec2<O&>{y, z}}
    {}

    template<typename O>
    T_Vec3<T>& operator=(const T_Vec3<O> &other)
    {
        x = T(other.x);
        y = T(other.y);
        z = T(other.z);
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
    T_Vec3<ld_t> operator/(const T_Vec3<O> &other) const
    {
        return T_Vec3<ld_t>{(ld_t)x/other.x, (ld_t)y/other.y, (ld_t)z/other.z};
    }

    T_Vec3<ld_t> operator*(const ld_t scalar) const
    {
        return T_Vec3<ld_t>{(ld_t)(x*scalar), (ld_t)(y*scalar), (ld_t)(z*scalar)};
    }

    T_Vec3<ld_t> operator/(const ld_t scalar) const
    {
        return T_Vec3<ld_t>{(ld_t)(x/scalar), (ld_t)(y/scalar), (ld_t)(z/scalar)};
    }

    template<typename O>
    bool operator==(const T_Vec3<O> &other) const
    {
        return x == other.x && y == other.y && z == other.z;
    }

    ld_t Mag()
    {
        return hypot(x, y, z);
    }

    T_Vec3<ld_t> Norm()
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
    const T_Vec3<T&> xyz, xyw, xzw, yzw, rgb;

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
        , rgb{T_Vec3<T&>{x, y, z}}
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
        , rgb{T_Vec3<T&>{x, y, z}}
    {}

    template<typename O>
    T_Vec4(const T_Vec4<O> &other)
        : x{T(other.x)}
        , y{T(other.y)}
        , z{T(other.z)}
        , w{T(other.w)}
        , r{x}
        , g{y}
        , b{z}
        , a{w}
        , xy{T_Vec2<O&>{x, y}}
        , xz{T_Vec2<O&>{x, z}}
        , xw{T_Vec2<O&>{x, w}}
        , yz{T_Vec2<O&>{y, z}}
        , yw{T_Vec2<O&>{y, w}}
        , zw{T_Vec2<O&>{z, w}}
        , xyz{T_Vec3<O&>{x, y, z}}
        , xyw{T_Vec3<O&>{x, y, w}}
        , xzw{T_Vec3<O&>{x, z, w}}
        , yzw{T_Vec3<O&>{y, z, w}}
        , rgb{T_Vec3<O&>{x, y, z}}
    {}

    template<typename O>
    T_Vec4<T>& operator=(const T_Vec4<O> &other)
    {
        x = T(other.x);
        y = T(other.y);
        z = T(other.z);
        w = T(other.w);
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
    T_Vec4<ld_t> operator/(const T_Vec4<O> &other) const
    {
        return T_Vec4<ld_t>{(ld_t)x/other.x, (ld_t)y/other.y, (ld_t)z/other.z, (ld_t)w/other.w};
    }

    T_Vec4<ld_t> operator*(const ld_t scalar) const
    {
        return T_Vec4<ld_t>{(ld_t)(x*scalar), (ld_t)(y*scalar), (ld_t)(z*scalar), (ld_t)(w*scalar)};
    }

    T_Vec4<ld_t> operator/(const ld_t scalar) const
    {
        return T_Vec4<ld_t>{(ld_t)(x/scalar), (ld_t)(y/scalar), (ld_t)(z/scalar), (ld_t)(w/scalar)};
    }

    template<typename O>
    bool operator==(const T_Vec4<O> &other) const
    {
        return x == other.x && y == other.y && z == other.z && w == other.w;
    }

    ld_t Mag()
    {
        return hypot(x, y, z, w);
    }

    T_Vec4<ld_t> Norm()
    {
        return (*this)/Mag();
    }
};

// Math
typedef T_Vec2<ll_t> Vec2;
typedef T_Vec3<ll_t> Vec3;
typedef T_Vec4<ll_t> Vec4;
typedef T_Vec2<ld_t> fVec2;
typedef T_Vec3<ld_t> fVec3;
typedef T_Vec4<ld_t> fVec4;
typedef T_Vec2<ull_t> uVec2;
typedef T_Vec3<ull_t> uVec3;
typedef T_Vec4<ull_t> uVec4;

typedef T_Vec3<ld_t> Rot;

// Color
typedef T_Vec3<byte_t> RGB;
typedef T_Vec4<byte_t> RGBA;
typedef T_Vec3<byte_t&> RGBRef;
typedef T_Vec4<byte_t&> RGBARef;
typedef T_Vec3<ld_t> HSV;

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

const ld_t DEGPERPI = 180/3.14159265;
HSV ToHSV(const RGB &rgb);
RGB ToRGB(const HSV &hsv);
