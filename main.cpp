#include "render2d.h"
#include "exampleshaders.h"

#include <iostream>

using namespace std;

int main()
{
    auto spScene = make_shared<Scene2d>(60, 6.28);

    Render2d r2d(2560, 1440, spScene);
    spScene->queueShader(multibrot);

    r2d.renderAll()->output("output");
}
