#include "render2d.h"
#include "exampleshaders.h"

#include <iostream>

using namespace std;

int main()
{
    Render2d r2d(2560, 1440, make_shared<Scene2d>());
    r2d.queueShader(mandelbrot);
    r2d.render()->output("mandel");

    auto spScene = make_shared<Scene2d>(60, 2, RGB{20, 180, 255});
    Render2d r2d2(2560, 1440, spScene);
    auto spSprite = make_shared<Sprite>("mandel.png");
    spSprite->queueShader(multibrot);
    spSprite->queueShader(rainbow);
    spScene->addActor(spSprite, Vec2{500, 0}, uVec2{500, 500});
    spScene->addActor(spSprite, Vec2{500, 0}, uVec2{100, 100});
    r2d2.renderAll()->output("output");
}
