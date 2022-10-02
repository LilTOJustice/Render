#pragma once

#include <vector>
#include <memory>

#include "shapes.h"

class Scene3d
{
    struct Actor
    {
        Vec3 pos, rot, vel;
    };

    public:
    void AddActor(std::shared_ptr<Solid>, const Vec3 pos, const Rot rot = {}, const Vec3 vel = {});

    private:
    std::vector<Actor> m_actors;
};
