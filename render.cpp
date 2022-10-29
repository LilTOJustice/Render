#include <stdexcept>
#include <iostream>
#include <chrono>

#include "render.h"

using namespace std;

Render2d::Render2d(long long xRes, long long yRes, std::shared_ptr<Scene2d> spScene)
    : m_xRes{xRes}, m_yRes{yRes}, m_spScene{spScene}
{
    if (m_spScene == nullptr)
    {
        throw runtime_error("Invalid scene provided to renderer.");
    }

    if (xRes < 1 || yRes < 1)
    {
         throw std::runtime_error("Invalid image size.");
    }
}

shared_ptr<Frame> Render2d::Render(long double time, bool verbose)
{
    auto output = make_shared<Frame>(m_xRes, m_yRes);
    auto& scene = *m_spScene;
    auto& camera = scene.GetCamera();

    if (verbose)
    {
        cout << "Beginning actor render (" << scene.GetActors().size() << " total)...\n";
    }
    auto start = chrono::high_resolution_clock::now();
    for (const auto &a : scene.GetActors())
    {
        auto spSprite = a.m_spSprite;
        auto* pixMap = spSprite->GetPixMap();
        unsigned int width = spSprite->GetWidth(), height = spSprite->GetHeight();
        cout << "Pos: (" << a.pos.x << ", " << a.pos.y << ")\n";
        cout << "Center: (" << camera.center.x << ", " << camera.center.y << ")\n";
        camera.zoom = 100;
        cout << "Zoom: " << camera.zoom << "\n";

        for (unsigned int i = 0; i < height; i++)
        {
            for (unsigned int j = 0; j < width; j++)
            {
                fVec2 transformed = camera(
                    Vec2{
                        a.pos.y+i-height/2,
                        a.pos.x+j-width/2
                    }
                );

                if (transformed.x == -1 && transformed.y == -1)
                {
                    continue;
                }

                transformed.x *= output->GetWidth();
                transformed.y *= output->GetHeight();

                RGBRef color = (*output.get())[transformed.y*output->GetWidth() + transformed.x];
                color.r = 255;
                color.g = 255;
                color.b = 255;
            }
        }
    }
    if (verbose)
    {
        cout << "Done! (" << chrono::duration_cast<chrono::duration<double>>(chrono::high_resolution_clock::now()-start).count() << "s)\n";
    }

    // Final screen-space shader pass
    if (verbose)
    {
        cout << "Beginning screen-space shader pass...\n";
    }
    start = chrono::high_resolution_clock::now();
    size_t numShaders = m_shaderQueue.size();
    Vec2 screenRes(m_xRes, m_yRes);
    for (size_t i = 0; i < m_shaderQueue.size(); i++)
    {
        FragShader shader = m_shaderQueue.front();
        if (verbose)
        {
            cout << "Computing shaders: (" << (m_shaderQueue.size()-numShaders) + 1 << " of " << numShaders << ")\n";
        }
        for (size_t i = 0; i < output->GetHeight(); i++)
        {
            for (size_t j = 0; j < output->GetWidth(); j++)
            {
                shader((*output.get())[i*output->GetWidth()+j], Vec2(j, (output->GetHeight()-1)-i), screenRes, time);
            }
        }

        // Pop and repush
        m_shaderQueue.pop();
        m_shaderQueue.push(shader);
    }
    if (verbose)
    {
        cout << "Done! (" << chrono::duration_cast<chrono::duration<double>>(chrono::high_resolution_clock::now()-start).count() << "s)\n";
    }

    return output;
}

shared_ptr<Movie> Render2d::RenderAll()
{
    auto spMovie = make_shared<Movie>(m_xRes, m_yRes, m_spScene->GetFps(), m_spScene->GetTimeSeq().size());
    for (const unsigned long long time : m_spScene->GetTimeSeq())
    {
        spMovie->WriteFrame(Render(time, false));
    }

    return spMovie;
}

void Render2d::QueueShader(const FragShader& shader)
{
    m_shaderQueue.push(shader);
}
