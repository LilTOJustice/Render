#include <stdexcept>

#include "render.h"

using namespace std;

shared_ptr<Image> Render2d::Render()
{
    if (m_spScene == nullptr)
    {
        throw runtime_error("Invalid scene provided to renderer.");
    }

    auto output = make_shared<Image>(m_xRes, m_yRes);
    auto& scene = *m_spScene;
    auto& camera = scene.GetCamera();

    //fVec2 normScreenCoords = Vec2{i, j}.Norm();
    //Vec2 transCoords = camera.ScreenToWorld(normScreenCoords);
    for (const Scene2d::Actor &a : scene.m_actors)
    {
        auto spSprite = a.m_spSprite;
        auto* pixMap = spSprite->GetPixMap();
        auto width = spSprite->GetWidth(), height = spSprite->GetHeight();

        for (auto i = 0; i < height; i++)
        {
            for (auto j = 0; j < width; j++)
            {
            }
        }
    }

    return output;
}
