#include "scene3d.h"

void Scene3d::AddActor(std::shared_ptr<Solid> spSolid, const Vec3 pos, const fVec3 rot, const Vec3 vel)
{
    m_actors.push_back(Actor{spSolid, pos, rot, vel});
}

Scene3d::Camera& Scene3d::GetCamera()
{
    return m_camera;
}

fVec2 Scene3d::Camera::operator()(const Actor &a)
{
    return fVec2{};
}
