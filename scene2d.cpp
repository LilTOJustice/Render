#include "scene2d.h"

void Scene2d::AddActor(std::shared_ptr<Sprite> spSprite, const Vec2 pos, const double rot, const Vec2 vel)
{
    m_actors.push_back(Actor{spSprite, pos, rot, vel});
}

Scene2d::Camera& Scene2d::GetCamera()
{
    return m_camera;
}

fVec2 Scene2d::Camera::operator()(const Vec2 &pixCoord)
{
    fVec2 result = (center-pixCoord)/zoom;

    if (result.x < 0 || result.x > 1 || result.y < 0 || result.y > 1)
    {
        return fVec2{-1, -1};
    }

    return result;
}

