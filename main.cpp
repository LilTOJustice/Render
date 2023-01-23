#include "render.h"
#include "exampleshaders.h"

#include <iostream>

using namespace std;

int main()
{
    auto spScene = make_shared<Scene2d>(165, 1);
    Render2d r2d(2560, 1440, spScene);
    r2d.QueueShader(multibrot);
    r2d.RenderAll()->Output("output");
}
