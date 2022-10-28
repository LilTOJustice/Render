#pragma once

#include <vector>
#include <memory>

#include "shapes.h"

class Scene3d
{
    struct Actor
    {
        std::shared_ptr<Solid> m_spSolid;
        Vec3 pos;
        fVec3 rot;
        Vec3 vel;
    };

    struct Camera
    {
        fVec2 operator()(const Actor &a);

        Vec3 center;
        double zoom;
        fVec3 rot;
    };

    public:
    void AddActor(std::shared_ptr<Solid> spSolid, const Vec3 pos, const fVec3 rot = {}, const Vec3 vel = {});

    Camera& GetCamera();

    private:
    std::vector<Actor> m_actors;
    Camera m_camera;

    friend class Render3d;
};
