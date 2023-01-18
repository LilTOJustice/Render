#include "scene2d.h"

using namespace std;

Scene2d::Scene2d(unsigned int fps, ld_t duration)
    : m_fps{fps}, m_camera{Vec2{0, 0}, 1, 0}
{
    if (fps > 0)
    {
        ld_t dt = 1./fps;
        for (ld_t t = 0; t <= duration-dt; t += dt)
        {
            m_timeSeq.push_back(t);
        }
    }
}

void Scene2d::AddActor(shared_ptr<Sprite> spSprite, Vec2 pos, uVec2 size, ld_t rot, Vec2 vel)
{
    if (size == uVec2{0, 0})
    {
        size = uVec2{spSprite->GetWidth(), spSprite->GetHeight()};
    }

    m_actors.push_back(
        Actor{
            spSprite, pos, size, rot, vel
        }
    );
}

ull_t Scene2d::GetFps() const
{
    return m_fps;
}

Scene2d::Camera& Scene2d::GetCamera()
{
    return m_camera;
}

const vector<ld_t>& Scene2d::GetTimeSeq() const
{
    return m_timeSeq;
}

vector<Scene2d::Actor>& Scene2d::GetActors()
{
    return m_actors;
}

fVec2 Scene2d::Camera::sstransform(const Vec2 &pixCoord)
{
    fVec2 result = (center-pixCoord)/zoom + fVec2{0.5, 0.5};

    if (result.x < 0 || result.x >= 1 || result.y < 0 || result.y >= 1)
    {
        return fVec2{-1, -1};
    }

    return result;
}
