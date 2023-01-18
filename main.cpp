#include "render.h"
#include "exampleshaders.h"

using namespace std;

int main()
{
    auto spScene = make_shared<Scene2d>(60, 60);
    //auto spSprite = make_shared<Sprite>(100, 300);
    Render2d r2d(2560, 1440, spScene);
    r2d.QueueShader(multibrot);
    r2d.RenderAll()->Output("output");
}
