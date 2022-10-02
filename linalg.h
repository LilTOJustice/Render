#pragma once

template<typename T>
struct T_Vec3
{
    T x, y, z;

    template<typename O>
    T_Vec3<T> operator+(T_Vec3<O> other) const
    {
        return T_Vec3{T(x+other.x), T(y+other.y), T(z+other.z)};
    }

    template<typename O>
    T_Vec3<T> operator-(T_Vec3<O> other) const
    {
        return T_Vec3{T(x-other.x), T(y-other.y), T(z-other.z)};
    }

    T_Vec3 operator*(const long double scalar) const
    {
        return T_Vec3{x*scalar, y*scalar, z*scalar};
    }

    T_Vec3<long double> operator/(const long double scalar) const
    {
        return T_Vec3{x/scalar, y/scalar, z/scalar};
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
struct T_Vec2
{
    T x, y;

    template<typename O>
    T_Vec2<T> operator+(T_Vec2<O> other) const
    {
        return T_Vec2{T(x+other.x), T(y+other.y)};
    }

    template<typename O>
    T_Vec2<T> operator-(T_Vec2<O> other) const
    {
        return T_Vec2{T(x-other.x), T(y-other.y)};
    }

    T_Vec2 operator*(const long double scalar) const
    {
        return T_Vec2{x*scalar, y*scalar};
    }

    T_Vec2<long double> operator/(const long double scalar) const
    {
        return T_Vec2{x/scalar, y/scalar};
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

typedef T_Vec3<long long> Vec3;
typedef T_Vec2<long long> Vec2;
typedef T_Vec3<long double> fVec3;
typedef T_Vec2<long double> fVec2;
typedef T_Vec3<long double> Rot;

