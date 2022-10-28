#include "render.h"
#include <complex>
#include <iostream>

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

FragShader mandelbrot =
Frag
{
    fVec2 st = FragCoord/Res;
    st = (st-fVec2(.7, .5))/.2;
    std::complex<long double> c(st.x, st.y);
    int mandelval = 1.*(MAXITS-mandel(c))/MAXITS*255;
    out = RGB(1-mandelval, mandelval, mandelval);
};

FragShader empty =
Frag
{};

int main()
{
    auto spScene = std::make_shared<Scene2d>();
    Render2d r2d(5000, 5000, spScene);
    r2d.QueueShader(mandelbrot);
    r2d.Render()->Output("output");
}
