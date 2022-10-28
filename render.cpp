#include <stdexcept>
#include <iostream>
#include <chrono>

#include "render.h"

using namespace std;

shared_ptr<Frame> Render2d::Render()
{
    if (m_spScene == nullptr)
    {
        throw runtime_error("Invalid scene provided to renderer.");
    }

    auto output = make_shared<Frame>(m_xRes, m_yRes);
    auto& scene = *m_spScene;
    auto& camera = scene.GetCamera();


    cout << "Beginning actor render (" << scene.m_actors.size() << " total)...\n";
    auto start = chrono::high_resolution_clock::now();
    for (const Scene2d::Actor &a : scene.m_actors)
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
    cout << "Done! (" << chrono::duration_cast<chrono::duration<double>>(chrono::high_resolution_clock::now()-start).count() << "s)\n";
    
    // Final screen-space shader pass
    cout << "Beginning screen-space shader pass...\n";
    start = chrono::high_resolution_clock::now();
    size_t numShaders = m_shaderQueue.size();
    Vec2 screenRes(m_xRes, m_yRes);
    while (!m_shaderQueue.empty())
    {
        FragShader shader = m_shaderQueue.front();
        cout << "Computing shaders: (" << (m_shaderQueue.size()-numShaders) + 1 << " of " << numShaders << ")\n";
        for (size_t i = 0; i < output->GetHeight(); i++)
        {
            for (size_t j = 0; j < output->GetWidth(); j++)
            {
                shader((*output.get())[i*output->GetWidth()+j], Vec2(j, (output->GetHeight()-1)-i), screenRes);
            }
        }

        m_shaderQueue.pop();
    }
    cout << "Done! (" << chrono::duration_cast<chrono::duration<double>>(chrono::high_resolution_clock::now()-start).count() << "s)\n";

    return output;
}

void Render2d::QueueShader(const FragShader& shader)
{
    m_shaderQueue.push(shader);
}
