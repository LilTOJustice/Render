#pragma once

#include <memory>
#include <stdexcept>
#include <thread>

#include "frame.h"
#include "scene2d.h"
#include "scene3d.h"
#include "shader.h"

class Render2d
{
    Render2d(const Render2d &) = delete;
    Render2d& operator=(const Render2d &) = delete;

    public:
    Render2d(ull_t xRes, ull_t yRes, std::shared_ptr<Scene2d> spScene, ull_t numThreads = std::thread::hardware_concurrency());

    std::shared_ptr<Frame> Render(ld_t time = 0, bool verbose = true);
    std::shared_ptr<Movie> RenderAll();
    std::shared_ptr<Scene2d> GetScene();
    void QueueShader(const FragShader &shader);
    void SetNumThreads(ull_t numThreads);

    private:
    ull_t m_xRes, m_yRes;
    std::shared_ptr<Scene2d> m_spScene;
    std::vector<FragShader> m_shaderQueue; // Functions like a queue
    ull_t m_numThreads;
};

/* TODO: Complete Render2d first
class Render3d
{
    Render3d(const Render3d &) = delete;
    Render3d& operator=(const Render3d &) = delete;

    public:
    std::shared_ptr<Frame> Render();
    void QueueShader(const FragShader &shader);

    Render3d(ull_t xRes, ull_t yRes, std::shared_ptr<Scene3d> spScene)
        : m_xRes{xRes}, m_yRes{yRes}, m_spScene{spScene}
    {
        if (xRes < 1 || yRes < 1)
        {
            throw std::runtime_error("Invalid image size.");
        }
    }

    private:
    ull_t m_xRes, m_yRes;
    std::shared_ptr<Scene3d> m_spScene;
    std::vector<FragShader> m_shaderQueue;
};
*/
