#pragma once

#include <vector>
#include <memory>

#include "shapes.h"

class Scene2d
{
    struct Actor
    {
        std::shared_ptr<Sprite> m_spSprite;
        Vec2 pos;
        double rot;
        Vec2 vel;
    };

    struct Camera
    {
        fVec2 operator()(const Actor &a);

        Vec2 center;
        double zoom;
        double rot;
    };

    public:
    void AddActor(std::shared_ptr<Sprite> spSprite, const Vec2 pos, const double rot = {}, const Vec2 vel = {});

    Camera& GetCamera();

    private:
    std::vector<Actor> m_actors;
    Camera m_camera;

    friend class Render2d;
};
