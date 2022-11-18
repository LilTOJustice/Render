#include "render.h"

#include <complex>
#include <cmath>
#include <iostream>

using namespace std;

const int MAXITS = 50;
int mandel(const complex<long double> &c)
{
    int its = 0;
    complex<long double> res = c;
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

FragShader mandelbrot =
Frag
{
    fVec2 st = FragCoord/Res;
    st.x *= 1.*Res.x/Res.y;
    st = (st-fVec2(1.25, .5))/(.1*(sin(Time) + .7) + .3);
    complex<long double> c(st.x, st.y);
    long double mandelval = 255.*(MAXITS-mandel(c))/MAXITS;
    out = RGB(1-mandelval, mandelval, mandelval);
};


FragShader rainbow =
Frag
{
    out = ToRGB(HSV(fmod(180*Time, 360), 1, 1));
};

int main()
{
    auto spScene = make_shared<Scene2d>(60, 2*3.14159265358979);
    Render2d r2d(2560, 1440, spScene);
    r2d.QueueShader(mandelbrot);
    r2d.RenderAll()->Output("output");
}
