#include "scene2d.h"

#include <stdexcept>

using namespace std;

// Scene2d
Scene2d::Scene2d(ull_t fps, ld_t duration, const RGB &bgColor)
    : m_fps{fps}, m_camera{Vec2{0, 0}, 1, 0}, m_bgColor{bgColor}
{
    if (fps > 0)
    {
        m_dt = 1. / fps;
        for (ld_t t = 0; t <= duration - m_dt; t += m_dt)
        {
            m_timeSeq.push_back(t);
        }
    }
}

Scene2d::Scene2d(const RGB &bgColor)
    : m_camera{Vec2{0, 0}, 1, 0}, m_bgColor{bgColor}
{}

void Scene2d::addActor(const shared_ptr<Actor> &spActor)
{
    m_actors.insert(spActor);
}

void Scene2d::addActor(const std::vector<std::shared_ptr<Actor>> &spActors)
{
    for (const auto &spActor : spActors)
    {
        m_actors.insert(spActor);
    }
}

std::shared_ptr<Scene2d::Actor> Scene2d::addActor(const shared_ptr<Sprite> &spSprite, const Vec2 &pos, const uVec2 &size, ld_t rot)
{
    auto spActor = make_shared<Actor>(spSprite, pos, size, rot);
    m_actors.insert(spActor);
    return spActor;
}

bool Scene2d::removeActor(const std::shared_ptr<Actor> &spActor)
{
    return m_actors.erase(spActor);
}

ull_t Scene2d::getFps() const
{
    return m_fps;
}

Scene2d::Camera& Scene2d::getCamera()
{
    return m_camera;
}

const Scene2d::Camera& Scene2d::getCamera() const
{
    return m_camera;
}

const vector<ld_t>& Scene2d::getTimeSeq() const
{
    return m_timeSeq;
}

ld_t Scene2d::getDt() const
{
    return m_dt;
}

const unordered_set<shared_ptr<Scene2d::Actor>>& Scene2d::getActors() const
{
    return m_actors;
}

RGB Scene2d::getBgColor() const
{
    return m_bgColor;
}

const vector<FragShader>& Scene2d::getShaderQueue() const
{
    return m_shaderQueue;
}

void Scene2d::queueShader(const FragShader &shader)
{
    m_shaderQueue.push_back(shader);
}

void Scene2d::clearShaders()
{
    m_shaderQueue.clear();
}

Vec2 Scene2d::ssTransform(const uVec2 &screenSize, const uVec2 &pixCoord) const
{
    return (Vec2{ll_t(pixCoord.x - ll_t(screenSize.x / 2)),
            ll_t(ll_t(screenSize.y / 2) - pixCoord.y)} / m_camera.getZoom() + m_camera.getCenter()).rot(m_camera.getRot());
}

// Scene2d::Camera
Scene2d::Camera::Camera(const Vec2 &center, ld_t zoom, ld_t rot)
    : m_center{center}, m_zoom{zoom}, m_rot{rot}
{}

void Scene2d::Camera::setCenter(const Vec2 &center)
{
    m_center = center;
}

void Scene2d::Camera::setZoom(ld_t zoom)
{
    m_zoom = zoom;
}

void Scene2d::Camera::setRot(ld_t rot)
{
    m_rot = rot;
}

void Scene2d::Camera::translate(const Vec2 &change)
{
    m_center += change;
}

void Scene2d::Camera::scaleZoom(ld_t zoomScale)
{
    m_zoom *= zoomScale;
}

void Scene2d::Camera::rotate(ld_t change)
{
    m_rot += change;
}

const Vec2& Scene2d::Camera::getCenter() const
{
    return m_center;
}

ld_t Scene2d::Camera::getZoom() const
{
    return m_zoom;
}

ld_t Scene2d::Camera::getRot() const
{
    return m_rot;
}

// Scene2d::Actor
Scene2d::Actor::Actor(const shared_ptr<Sprite> &spSprite, Vec2 pos, uVec2 size, ld_t rot)
    : m_spSprite{spSprite}, m_pos{pos}, m_size{size}, m_rot{rot}
{
    if (!m_spSprite)
    {
        throw runtime_error("Actor passed invalid sprite.");
    }

    if (m_size == uVec2{0, 0})
    {
        m_size = uVec2{m_spSprite->getWidth(), m_spSprite->getHeight()};
    }
}

void Scene2d::Actor::setSprite(const shared_ptr<Sprite> &spSprite)
{
    m_spSprite = spSprite;
}

void Scene2d::Actor::setPos(const Vec2 &pos)
{
    m_pos = pos;
}

void Scene2d::Actor::setSize(const uVec2 &size)
{
    m_size = size;
}

void Scene2d::Actor::setRot(ld_t rot)
{
    m_rot = rot;
}

void Scene2d::Actor::translate(const Vec2 &change)
{
    m_pos += change;
}

void Scene2d::Actor::scale(const fVec2 &scale)
{
    m_size *= scale;
}

void Scene2d::Actor::rotate(ld_t change)
{
    m_rot += change;
}

shared_ptr<Sprite> Scene2d::Actor::getSprite() const
{
    return m_spSprite;
}

const Vec2& Scene2d::Actor::getPos() const
{
    return m_pos;
}

const uVec2& Scene2d::Actor::getSize() const
{
    return m_size;
}

ld_t Scene2d::Actor::getRot() const
{
    return m_rot;
}
