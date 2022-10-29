#pragma once

#include <functional>

#include "linalg.h"

// Frag shaders
#define FRAGPARAMLIST RGBRef out, const Vec2 &FragCoord, const Vec2 &Res, long double Time
#define Frag [](FRAGPARAMLIST)

typedef std::function<void(FRAGPARAMLIST)> FragShader;

// This is how to write a shader
/*
FragShader Test =
Frag {
    out = RGB{0, 0, 0};
};
*/
