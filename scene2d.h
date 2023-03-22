#pragma once

#include <vector>
#include <memory>

#include "shapes.h"

class Scene2d
{
    struct Camera
    {
        Vec2 sstransform(const uVec2 &pixCoord, const uVec2 &screenSize);

        Vec2 center;
        ld_t zoom;
        ld_t rot;
    };

    public:
    struct Actor
    {
        std::shared_ptr<Sprite> m_spSprite;
        Vec2 pos;
        uVec2 size;
        ld_t rot;
    };

    Scene2d(unsigned int framerate, ld_t duration, const RGB &bgColor = RGB{0, 0, 0}); // For Movie rendering or single-frame rendering
    Scene2d(const RGB &bgColor = RGB{0, 0, 0}); // For single-frame rendering

    void AddActor(std::shared_ptr<Sprite> spSprite, Vec2 pos = {}, uVec2 size = {}, ld_t rot = {});

    ull_t GetFps() const;

    Camera& GetCamera();

    const std::vector<ld_t>& GetTimeSeq() const;

    std::vector<Actor>& GetActors();

    RGB GetBgColor();

    private:
    ull_t m_fps;
    Camera m_camera;
    std::vector<ld_t> m_timeSeq;
    std::vector<Actor> m_actors;
    RGB m_bgColor;
};
