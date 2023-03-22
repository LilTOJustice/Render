#pragma once

#include <vector>
#include <memory>

#include "shapes.h"

class Scene2d
{

    public:
    class Camera
    {
        public:
        Camera(const Vec2 &center, ld_t zoom, ld_t rot);

        void setCenter(const Vec2 &center);
        void setZoom(ld_t zoom);
        void setRot(ld_t rot);

        const Vec2& getCenter() const;
        ld_t getZoom() const;
        ld_t getRot() const;

        private:
        Vec2 m_center;
        ld_t m_zoom;
        ld_t m_rot;
    };

    class Actor
    {
        public:
        Actor(std::shared_ptr<Sprite> spSprite = nullptr, Vec2 pos = {}, uVec2 size = {}, ld_t rot = 0);

        void setSprite(std::shared_ptr<Sprite> spSprite);
        void setPos(const Vec2 &pos);
        void setSize(const uVec2 &size);
        void setRot(ld_t rot);
        
        std::shared_ptr<Sprite> getSprite() const;
        const Vec2& getPos() const;
        const uVec2& getSize() const;
        ld_t getRot() const;

        private:
        std::shared_ptr<Sprite> m_spSprite;
        Vec2 m_pos;
        uVec2 m_size;
        ld_t m_rot;
    };

    Scene2d(ull_t framerate, ld_t duration, const RGB &bgColor = RGB{0, 0, 0}); // For Movie rendering or single-frame rendering
    Scene2d(const RGB &bgColor = RGB{0, 0, 0}); // For single-frame rendering

    void addActor(const Actor &actor);
    void addActor(std::shared_ptr<Sprite> spSprite, const Vec2 &pos = {}, const uVec2 &size = {}, ld_t rot = {});

    ull_t getFps() const;

    Camera& getCamera();
    const Camera& getCamera() const;

    const std::vector<ld_t>& getTimeSeq() const;

    const std::vector<Actor>& getActors() const;

    RGB getBgColor() const;

    const std::vector<FragShader>& getShaderQueue() const;

    void queueShader(const FragShader &shader);

    void clearShaders();

    Vec2 ssTransform(const uVec2 &screenSize, const uVec2 &pixCoord) const;

    private:
    ull_t m_fps;
    Camera m_camera;
    std::vector<ld_t> m_timeSeq;
    std::vector<Actor> m_actors;
    RGB m_bgColor;
    std::vector<FragShader> m_shaderQueue; // Functions like a queue
};
