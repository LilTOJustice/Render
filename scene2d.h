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
        uVec2 size;
        ld_t rot;
        Vec2 vel;
    };

    struct Camera
    {
        fVec2 sstransform(const Vec2 &pixCoord);

        Vec2 center;
        ld_t zoom;
        ld_t rot;
    };

    public:
    Scene2d(unsigned int framerate = 0, ld_t duration = 0);

    void AddActor(std::shared_ptr<Sprite> spSprite, Vec2 pos, uVec2 size = {}, ld_t rot = {}, Vec2 vel = {});

    ull_t GetFps() const;

    Camera& GetCamera();

    const std::vector<ld_t>& GetTimeSeq() const;

    std::vector<Actor>& GetActors();

    private:
    ull_t m_fps;
    Camera m_camera;
    std::vector<ld_t> m_timeSeq;
    std::vector<Actor> m_actors;
};
