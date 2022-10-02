#pragma once

#include <memory>
#include <stdexcept>

#include "image.h"
#include "scene2d.h"
#include "scene3d.h"

class Render2d
{
    Render2d(const Render2d &) = delete;
    Render2d& operator=(const Render2d &) = delete;

    public:
    std::shared_ptr<Image> Render();

    Render2d(long long xRes, long long yRes, std::shared_ptr<Scene2d> spScene)
        : m_xRes{xRes}, m_yRes{yRes}, m_spScene{spScene}
    {
        if (xRes < 1 || yRes < 1)
        {
             throw std::runtime_error("Invalid image size.");
        }
    }

    private:
    long long m_xRes, m_yRes;
    std::shared_ptr<Scene2d> m_spScene;
};

class Render3d
{
    Render3d(const Render3d &) = delete;
    Render3d& operator=(const Render3d &) = delete;
    std::shared_ptr<Image> Render();

    Render3d(long long xRes, long long yRes, std::shared_ptr<Scene3d> spScene)
        : m_xRes{xRes}, m_yRes{yRes}, m_spScene{spScene}
    {
        if (xRes < 1 || yRes < 1)
        {
             throw std::runtime_error("Invalid image size.");
        }
    }

    private:
    long long m_xRes, m_yRes;
    std::shared_ptr<Scene3d> m_spScene;
};

