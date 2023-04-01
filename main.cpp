#include "render2d.h"
#include "exampleshaders.h"

#include <iostream>

using namespace std;

int main()
{
    // 2d scene: 90 fps, 2s duration, background color light blue
    auto spBgSprite = make_shared<Sprite>("F24_6pack2_2560x1440.png");
    auto spScene = make_shared<Scene2d>(90, 2, RGB{20, 180, 255}, spBgSprite);

    // Renderer with screen size 2560x1440 bound to the scene
    Render2d r2d(2560, 1440, spScene);

    // Sprite with resolution 500x500 filled with white pixels
    auto spSprite = make_shared<Sprite>(uVec2{500, 500}, RGBA{255, 255, 255, 255});

    // Two actors sharing the sprite
    auto spAct1 = spScene->addActor(spSprite, Vec2{0, 0}, uVec2{500, 500}); // at (0, 0) size 500x500
    auto spAct2 = spScene->addActor(spSprite, Vec2{500, 0}, uVec2{100, 100}); // at (500, 0) size 100x100
    
    // Give the two actors the multibrot and rainbow shaders
    spAct1->queueShader(multibrot); // Multibrot will overwrite the white pixels
    spAct1->queueShader(rainbow);
    spAct2->queueShader(multibrot);
    spAct2->queueShader(rainbow);

    // This function runs between each frame
    SceneThinkFunc think =
    SceneThink(spScene, spAct1, spAct2) // time, dt
    {
        spAct1->translate(Vec2{1, 1});
        spAct2->translate(Vec2{0, -1});
        spAct1->rotate(2/DEGPERPI);
        spScene->getCamera().rotate(1/DEGPERPI);
        spScene->getCamera().translate(Vec2{1, 0});
    };

    // Inform the renderer that this needs to happen between each frame
    r2d.bindThinkFunc(think);

    // Render all frames as mp4
    r2d.renderAll()->output("output");
}
