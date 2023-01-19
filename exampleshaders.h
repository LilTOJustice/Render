#include <complex>
#include <cmath>

#include <iostream>

#include "shader.h"

const int MAXITS = 50;
int mandel(const std::complex<long double> &c)
{
    int its = 0;
    std::complex<long double> res = c;
    while (res.real() < 2 && res.imag() < 2)
    {
        res = res*res + c;
        if (its == MAXITS)
        {
            break;
        }

        its++;
    }
    return its;
}

int multi(const std::complex<long double> &c, const long double exponent)
{
    int its = 0;
    std::complex<long double> res = c;
    while (res.real() < 2 && res.imag() < 2)
    {
        res = std::pow(res, exponent) + c;
        if (its == MAXITS)
        {
            break;
        }

        its++;
    }
    return its;
}

FragShader mandelbrot =
Frag
{
    fVec2 st = FragCoord/Res;
    st.x *= 1.*Res.x/Res.y;
    st = (st-fVec2(1.25, .5))/(.1*(sin(time) + .7) + .3);
    std::complex<long double> c(st.x, st.y);
    long double mandelval = 255.*(MAXITS-mandel(c))/MAXITS;
    out = RGB(1-mandelval, mandelval, mandelval);
};

FragShader multibrot =
Frag
{
    fVec2 st = FragCoord/Res;
    st.x *= 1.*Res.x/Res.y;
    st = (st-fVec2(1.25, .5))/.3;
    std::complex<long double> c(st.x, st.y);
    long double mandelval = 255.*(MAXITS-multi(c, 3*cos(time)+4))/MAXITS;
    out = RGB(1-mandelval, mandelval, mandelval);
};

FragShader rainbow =
Frag
{
    out = ToRGB(HSV(fmod(180*time, 360), 1, 1));
};
