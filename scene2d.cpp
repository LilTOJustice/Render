#include "scene2d.h"

using namespace std;

Scene2d::Scene2d(unsigned int fps, ld_t duration, const RGB &bgColor)
    : m_fps{fps}, m_camera{Vec2{0, 0}, 1, 0}, m_bgColor{bgColor}
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

Scene2d::Scene2d(const RGB &bgColor)
    : m_camera{Vec2{0, 0}, 1, 0}, m_bgColor{bgColor}
{}

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

RGB Scene2d::GetBgColor()
{
    return m_bgColor;
}

Vec2 Scene2d::Camera::sstransform(const uVec2 &pixCoord, const uVec2 &screenSize)
{
    return (Vec2{ll_t(pixCoord.x - ll_t(screenSize.x / 2)), ll_t(ll_t(screenSize.y / 2) - pixCoord.y)} / zoom + center).Rot(rot);
}
