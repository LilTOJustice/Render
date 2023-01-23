#pragma once

#include <functional>

#include "linalg.h"

// Frag shaders
#define FRAGPARAMLIST const RGB in, RGBRef out, const uVec2 &fragcoord, const uVec2 &res, const ld_t time
#define Frag [](FRAGPARAMLIST)

typedef std::function<void(FRAGPARAMLIST)> FragShader;

// This is how to write a shader
/*
FragShader Test =
Frag {
    out = RGB{0, 0, 0};
};
*/
