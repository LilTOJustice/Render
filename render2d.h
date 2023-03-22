#pragma once

#include <memory>
#include <stdexcept>
#include <thread>

#include "frame.h"
#include "scene2d.h"
#include "shader.h"

class Render2d
{
    Render2d(const Render2d &) = delete;
    Render2d& operator=(const Render2d &) = delete;

    public:
    Render2d(ull_t xRes, ull_t yRes, std::shared_ptr<Scene2d> spScene, ull_t numThreads = std::thread::hardware_concurrency());

    std::shared_ptr<Frame> render(ld_t time = 0, bool verbose = true);
    std::shared_ptr<Movie> renderAll();
    std::shared_ptr<Scene2d> getScene();
    void setNumThreads(ull_t numThreads);

    private:
    ull_t m_xRes, m_yRes;
    std::shared_ptr<Scene2d> m_spScene;
    ull_t m_numThreads;
};
