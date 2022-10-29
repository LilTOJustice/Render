#pragma once

#include <vector>
#include <memory>
#include <queue>

#include "shapes.h"

class Scene2d
{
    struct Actor
    {
        std::shared_ptr<Sprite> m_spSprite;
        Vec2 pos;
        long double rot;
        Vec2 vel;
    };

    struct Camera
    {
        fVec2 operator()(const Vec2 &pixCoord);

        Vec2 center;
        long double zoom;
        long double rot;
    };

    public:
    Scene2d(unsigned int framerate = 0, long double time = 0);

    void AddActor(std::shared_ptr<Sprite> spSprite, const Vec2 pos, const long double rot = {}, const Vec2 vel = {});

    unsigned long long GetFps() const;

    Camera& GetCamera();

    const std::vector<long double>& GetTimeSeq() const;

    std::vector<Actor>& GetActors();

    private:
    unsigned long long m_fps;
    Camera m_camera;
    std::vector<long double> m_timeSeq;
    std::vector<Actor> m_actors;
};
