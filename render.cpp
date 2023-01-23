#include "render.h"

#include <atomic>
#include <chrono>
#include <iostream>
#include <queue>
#include <stdexcept>

#define NUMBARS 50

using namespace std;

const string loadSeq = "|/-\\";

Render2d::Render2d(ull_t xRes, ull_t yRes, std::shared_ptr<Scene2d> spScene, ull_t numThreads)
    : m_xRes{xRes}, m_yRes{yRes}, m_spScene{spScene}, m_numThreads{numThreads}
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

shared_ptr<Frame> Render2d::Render(ld_t time, bool verbose)
{
    auto output = make_shared<Frame>(m_xRes, m_yRes);
    auto& scene = *m_spScene;
    auto& camera = scene.GetCamera();

    if (verbose)
    {
        cout << "\nBeginning actor render (" << scene.GetActors().size() << " total)...\n";
    }

    auto start = chrono::high_resolution_clock::now();
    for (const auto &actor : scene.GetActors())
    {
        cout << "Pos: (" << actor.pos.x << ", " << actor.pos.y << ")\n";
        cout << "Center: (" << camera.center.x << ", " << camera.center.y << ")\n";
        camera.zoom = 1;
        cout << "Zoom: " << camera.zoom << "\n";

        auto spSprite = actor.m_spSprite;
        //auto* pixMap = spSprite->GetPixMap();
        ull_t width = spSprite->GetWidth(), height = spSprite->GetHeight();
        ull_t tWidth = width*camera.zoom, tHeight = height*camera.zoom;

        for (ull_t i = 0; i < tHeight; i++)
        {
            for (ull_t j = 0; j < tWidth; j++)
            {
                fVec2 transformed = camera.sstransform(
                    Vec2{
                        actor.pos.x+(ll_t)j-(ll_t)tWidth/2,
                        actor.pos.y+(ll_t)i-(ll_t)tHeight/2
                    }
                );

                if (transformed.x == -1 || transformed.y == -1)
                {
                    continue;
                }

                cout << "Transformed: (" << transformed.x << ", " << transformed.y << ")\n";

                transformed.x *= output->GetWidth();
                transformed.y *= output->GetHeight();

                RGBRef color = (*output.get())[transformed.y*output->GetWidth() + transformed.x];
                color = RGB(255, 255, 255);
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
        cout << "\nBeginning screen-space shader pass...\n";
        start = chrono::high_resolution_clock::now();
    }

    size_t numShaders = m_shaderQueue.size();
    uVec2 screenRes(m_xRes, m_yRes);

    for (ull_t shaderInd = 0; shaderInd < m_shaderQueue.size(); shaderInd++)
    {
        if (verbose)
        {
            cout << "Computing shaders: (" << shaderInd + 1 << " of " << numShaders << ")\n";
        }

        for (size_t i = 0; i < output->GetHeight(); i++)
        {
            for (size_t j = 0; j < output->GetWidth(); j++)
            {
                m_shaderQueue[shaderInd]((*output.get())[i*output->GetWidth()+j], uVec2{j, (output->GetHeight()-1)-i}, screenRes, time);
            }
        }
    }

    if (verbose)
    {
        cout << "Done! (" << chrono::duration_cast<chrono::duration<double>>(chrono::high_resolution_clock::now()-start).count() << "s)\n";
    }

    return output;
}

struct jobinfo
{
    ld_t time;
    ull_t frameIndex;
};

shared_ptr<Scene2d> Render2d::GetScene()
{
    return m_spScene;
}

void ThreadRender(Render2d *_this, shared_ptr<Movie> spMovie, atomic_ullong *aFrameIndex)
{
    ull_t numFrames = spMovie->GetNumFrames();
    for (ull_t frameInd = (*aFrameIndex)++; frameInd < numFrames; frameInd = (*aFrameIndex)++)
    {
        spMovie->WriteFrame(_this->Render(_this->GetScene()->GetTimeSeq()[frameInd], false), frameInd);
    }
}

void printBar(ull_t frameIndex, ull_t numFrames, ull_t totalBars)
{
    static ull_t loadSeqInd = 0;
    ull_t numBars = 1.*frameIndex/numFrames*totalBars;

    cout << "\r[";
    for (ull_t i = 0; i < numBars; i++)
    {
        cout << '|';
    }

    for (ull_t i = 0; i < totalBars - numBars; i++)
    {
        cout << ' ';
    }

    cout << "] " << frameIndex << '/' << numFrames << " (" << fixed << 100.*frameIndex/numFrames << ")% " << loadSeq[(loadSeqInd++) % loadSeq.size()];
    cout.flush();
}

shared_ptr<Movie> Render2d::RenderAll()
{
    if (m_numThreads <= 0)
    {
        throw runtime_error("Invalid number of threads given (" + to_string(m_numThreads) + ")!");
    }

    auto spMovie = make_shared<Movie>(m_xRes, m_yRes, m_spScene->GetFps(), m_spScene->GetTimeSeq().size());
    ull_t numFrames = spMovie->GetNumFrames();
    cout << "\nBeginning render " << '(' << m_numThreads << " thread" << (m_numThreads > 1 ? "s" : "") << "): "
        << spMovie->GetWidth() << 'x' << spMovie->GetHeight() << " @ " << spMovie->GetFps() << " -> "
        << numFrames << " frames (" << spMovie->GetDuration() << "s)\n";

    auto start = chrono::high_resolution_clock::now();
    atomic_ullong aFrameIndex(0);
    vector<thread> renderThreads;

    for (ull_t i = 0; i < m_numThreads; i++)
    {
        renderThreads.emplace_back(ThreadRender, this, spMovie, &aFrameIndex);
    }

    // Wait for completion
    while (aFrameIndex < numFrames)
    {
        printBar(aFrameIndex, numFrames, NUMBARS);
        this_thread::sleep_for(chrono::milliseconds(300));
    }

    for (auto &rt : renderThreads)
    {
        rt.join();
    }

    printBar(numFrames, numFrames, NUMBARS);

    cout << "\nDone! (" << chrono::duration_cast<chrono::duration<double>>(chrono::high_resolution_clock::now()-start).count() << "s)\n";
    return spMovie;
}

void Render2d::QueueShader(const FragShader &shader)
{
    m_shaderQueue.push_back(shader);
}

void Render2d::SetNumThreads(ull_t numThreads)
{
    m_numThreads = numThreads;
}
