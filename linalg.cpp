#include "linalg.h"

// Color functions
HSV ToHSV(const RGB &rgb)
{
    long double R = rgb.r, G = rgb.g, B = rgb.b;
    long double M = max(max(R, G), B);
    long double m = min(min(R, G), B);
    long double V = M/255;
    long double S = (M > 0 ? 1 - M/m : 0);
    long double H = acos((R - .5*G - .5*B)/sqrt(R*R + G*G + B*B - R*G - R*B - G*B))*DEGPERPI;
    if (B > G)
    {
        H = 360 - H;
    }

    return HSV(H, S, V);
}

RGB ToRGB(const HSV &hsv)
{
    long double H = hsv.h, S = hsv.s, V = hsv.v;
    long double M = 255*V;
    long double m = M*(1-S);
    long double z = (M-m)*(1 - fabs(fmod(H/60, 2) - 1));
    unsigned char R, G, B;
    if (H < 60)
    {
        R = M;
        G = z + m;
        B = m;
    }
    else if (H < 120)
    {
        R = z + m;
        G = M;
        B = m;
    }
    else if (H < 180)
    {
        R = m;
        G = M;
        B = z + m;
    }
    else if (H < 240)
    {
        R = m;
        G = z + m;
        B = M;
    }
    else if (H < 300)
    {
        R = z + m;
        G = m;
        B = M;
    }
    else
    {
        R = M;
        G = m;
        B = z + m;
    }
    return RGB(R, G, B);
}
