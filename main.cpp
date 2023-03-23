#include "render2d.h"
#include "exampleshaders.h"

#include <iostream>

using namespace std;

int main()
{
    auto spScene = make_shared<Scene2d>(90, 2, RGB{20, 180, 255});
    Render2d r2d(2560, 1440, spScene);
    auto spSprite = make_shared<Sprite>(2560, 1440);
    auto spAct1 = spScene->addActor(spSprite, Vec2{500, 0}, uVec2{500, 500});
    auto spAct2 = spScene->addActor(spSprite, Vec2{500, 0}, uVec2{100, 100});
    auto spLine = spScene->addLine(Vec2{0, 0}, Vec2{300, 300}, RGBA{255, 0, 0, 255}, 10);
    spSprite->queueShader(multibrot);
    spSprite->queueShader(rainbow);

    SceneThinkFunc think =
    SceneThink(spScene, spAct1, spAct2) // time, dt
    {
        spAct1->translate(Vec2{-1, 0});
        spAct2->translate(Vec2{0, -2});
    };

    r2d.bindThinkFunc(think);
    r2d.renderAll()->output("output");
}
