#pragma once

#include <memory>
#include <stdexcept>
#include <thread>
#include <vector>

#include "frame.h"
#include "scene2d.h"
#include "shader.h"

#define SCENETHINKPARAMLIST ld_t time, ld_t dt
#define SceneThink(CAPTUREPARAMS ...) [CAPTUREPARAMS](SCENETHINKPARAMLIST)

typedef std::function<void(SCENETHINKPARAMLIST)> SceneThinkFunc;

class Render2d
{
    Render2d(const Render2d &) = delete;
    Render2d& operator=(const Render2d &) = delete;

    public:
    class SceneInstance
    {
        public:
        SceneInstance(const Scene2d &scene);

        Scene2d::Camera& getCamera();
        const Scene2d::Camera& getCamera() const;

        const std::vector<Scene2d::Actor>& getActors() const;

        RGB getBgColor() const;

        const std::vector<FragShader>& getShaderQueue() const;

        Vec2 ssTransform(const uVec2 &screenSize, const uVec2 &pixCoord) const;

        private:
        Scene2d::Camera m_camera;
        std::vector<Scene2d::Actor> m_actors;
        RGB m_bgColor;
        std::vector<FragShader> m_shaderQueue; // Functions like a queue
    };

    Render2d(ull_t xRes, ull_t yRes, const std::shared_ptr<Scene2d> &spScene, ull_t numThreads = std::thread::hardware_concurrency());

    std::shared_ptr<Frame> render(ld_t time = 0, bool verbose = true) const;
    std::shared_ptr<Frame> render(const SceneInstance &scene, ld_t time = 0, bool verbose = true) const;

    std::shared_ptr<Movie> renderAll() const;
    
    std::shared_ptr<Scene2d> getScene() const;
    
    void setNumThreads(ull_t numThreads);

    void bindThinkFunc(const SceneThinkFunc &stf);

    void unbindThinkFunc();

    private:
    ull_t m_xRes, m_yRes;
    std::shared_ptr<Scene2d> m_spScene;
    ull_t m_numThreads;
    SceneThinkFunc m_sceneThinkFunc;
};
