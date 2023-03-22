#include "render2d.h"
#include "exampleshaders.h"

#include <iostream>

using namespace std;

int main()
{
    auto spScene = make_shared<Scene2d>(60, 2, RGB{20, 180, 255});
    Render2d r2d(2560, 1440, spScene);
    auto spSprite = make_shared<Sprite>(2560, 1440);
    spSprite->queueShader(multibrot);
    spSprite->queueShader(rainbow);
    spScene->addActor(spSprite, Vec2{500, 0}, uVec2{500, 500});
    spScene->addActor(spSprite, Vec2{500, 0}, uVec2{100, 100});
    r2d.renderAll()->output("output");
}
