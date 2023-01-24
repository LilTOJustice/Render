#include "render.h"
#include "exampleshaders.h"

#include <iostream>

using namespace std;

int main()
{
    Render2d r2d(2560, 1440, make_shared<Scene2d>());
    r2d.QueueShader(mandelbrot);
    r2d.Render()->Output("mandel");

    auto spScene = make_shared<Scene2d>(165, 5, RGB{20, 180, 245});
    Render2d r2d2(2560, 1440, spScene);
    auto spSprite = make_shared<Sprite>("mandel.png");
    spSprite->QueueShader(rainbow);
    spScene->AddActor(spSprite, Vec2{500, 0}, uVec2{500, 500});
    spScene->AddActor(spSprite, Vec2{500, 0}, uVec2{100, 100});
    r2d2.Render()->Output("output");
}
