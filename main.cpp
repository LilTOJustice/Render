#include "render.h"
#include <iostream>

int main()
{
    auto spScene = std::make_shared<Scene2d>();
    Render2d r2d(2560, 1440, spScene);

    auto s = std::make_shared<Sprite>(200, 200);
    Color* pPixMap = s->GetPixMap();
    for (unsigned long long i = 0; i < s->GetHeight(); i++)
    {
        for (unsigned long long j = 0; j < s->GetWidth(); j++)
        {
            pPixMap[i*s->GetWidth()+j] = Color{255, 255, 255};
        }
    }
    spScene->AddActor(s, {0, 0});

    r2d.Render()->Output("image");
}
