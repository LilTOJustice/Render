#include "render.h"
#include "exampleshaders.h"

#include <iostream>

#define THREADS 12

using namespace std;

int main()
{
    auto spScene = make_shared<Scene2d>(60, 1);
    Render2d r2d(2560, 1440, spScene);
    r2d.QueueShader(mandelbrot);

    cout << "Rendering with 1 thread\n";
    r2d.RenderAll()->Output("output");

    cout << "Rendering with " << THREADS << " threads\n";
    r2d.SetNumThreads(THREADS);
    r2d.RenderAll()->Output("output");
}
