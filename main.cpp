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
    st = (st-fVec2(1.25, .5))/(.4*(sin(Time)/2 + .5) + .1);
    complex<long double> c(st.x, st.y);
    long double mandelval = 1.*(MAXITS-mandel(c))/MAXITS;
    out = ToRGB(HSV(fmod(180*Time, 360), 1, 1))*mandelval;
    out.r = 1-out.r;
    out.g = 1-out.g;
    out.b = 1-out.b;
};


FragShader rainbow =
Frag
{
    out = ToRGB(HSV(fmod(180*Time, 360), 1, 1));
};

int main()
{
    auto spScene = make_shared<Scene2d>(60, 2);
    Render2d r2d(2560, 1440, spScene);
    r2d.QueueShader(rainbow);
    r2d.RenderAll()->Output("output");
}
