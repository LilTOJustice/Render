#include "scene2d.h"

void Scene2d::AddActor(std::shared_ptr<Sprite> spSprite, const Vec2 pos, const double rot, const Vec2 vel)
{
    m_actors.push_back(Actor{spSprite, pos, rot, vel});
}

Scene2d::Camera& Scene2d::GetCamera()
{
    return m_camera;
}

fVec2 Scene2d::Camera::operator()(const Actor &a)
{
    return (pos-a.pos)/zoom;
}
