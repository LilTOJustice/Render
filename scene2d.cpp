#include "scene2d.h"

using namespace std;

Scene2d::Scene2d(unsigned int fps, long double time)
    : m_fps{fps}, m_camera{Vec2{0, 0}, 1, 0}
{
    if (fps > 0)
    {
        long double dt = 1./fps;
        for (long double t = 0; t <= time-dt; t += dt)
        {
            m_timeSeq.push_back(t);
        }
    }
}

void Scene2d::AddActor(shared_ptr<Sprite> spSprite, const Vec2 pos, const long double rot, const Vec2 vel)
{
    m_actors.push_back(Actor{spSprite, pos, rot, vel});
}

unsigned long long Scene2d::GetFps() const
{
    return m_fps;
}

Scene2d::Camera& Scene2d::GetCamera()
{
    return m_camera;
}

const vector<long double>& Scene2d::GetTimeSeq() const
{
    return m_timeSeq;
}

vector<Scene2d::Actor>& Scene2d::GetActors()
{
    return m_actors;
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
