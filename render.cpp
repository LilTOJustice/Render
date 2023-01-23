#include "render.h"

#include <atomic>
#include <chrono>
#include <iomanip>
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
    uVec2 screenRes(m_xRes, m_yRes);

    if (verbose)
    {
        cout << "\nBeginning actor render (" << scene.GetActors().size() << " total)...\n";
    }

    auto start = chrono::high_resolution_clock::now();
    for (ull_t i = 0; i < m_yRes; i++)
    {
        for (ull_t j = 0; j < m_xRes; j++)
        {
            size_t arrayLoc = i*output->GetWidth() + j;
            (*output.get())[arrayLoc] = scene.GetBgColor();
            Vec2 worldCoord = camera.sstransform(uVec2{j, i}, screenRes); 

            for (const auto &actor : scene.GetActors())
            {
                Vec2 bottomRight = actor.pos + Vec2{ll_t(actor.size.x / 2), -ll_t(actor.size.y / 2)};
                Vec2 topLeft = actor.pos + Vec2{-ll_t(actor.size.x / 2), ll_t(actor.size.y / 2)};

                if (worldCoord.x <= bottomRight.x
                        && worldCoord.x >= topLeft.x
                        && worldCoord.y >= bottomRight.y
                        && worldCoord.y <= topLeft.y)
                {
                    auto spSprite = actor.m_spSprite;
                    auto* pixMap = spSprite->GetPixMap();
                    uVec2 pixMapSize{spSprite->GetWidth(), spSprite->GetHeight()};

                    uVec2 pixMapInd = ((worldCoord - topLeft) / (bottomRight - topLeft)) * uVec2{spSprite->GetWidth() - 1, spSprite->GetHeight() - 1};
                    RGB color = pixMap[pixMapInd.y * spSprite->GetWidth() + pixMapInd.x].rgb;

                    for (const FragShader &fs : spSprite->GetShaderQueue())
                    {
                        fs(color, color, uVec2{pixMapInd.x, (spSprite->GetHeight() - 1) - pixMapInd.y}, pixMapSize, time);
                    }

                    (*output.get())[arrayLoc] = color;
                }
            }
        }
    }

    if (verbose)
    {
        cout << "Done! (" << chrono::duration_cast<chrono::duration<double>>(chrono::high_resolution_clock::now()-start).count() << "s)\n";

        // Final screen-space shader pass
        cout << "\nBeginning screen-space shader pass...\n";
        start = chrono::high_resolution_clock::now();
    }


    for (const FragShader &fs : m_shaderQueue)
    {
        for (size_t i = 0; i < output->GetHeight(); i++)
        {
            for (size_t j = 0; j < output->GetWidth(); j++)
            {
                fs((*output.get())[i * output->GetWidth() + j], (*output.get())[i * output->GetWidth() + j], uVec2{j, (output->GetHeight() - 1) - i}, screenRes, time);
            }
        }
    }

    if (verbose)
    {
        cout << "Done! (" << chrono::duration_cast<chrono::duration<double>>(chrono::high_resolution_clock::now()-start).count() << "s)\n";
    }

    return output;
}

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

void PrintBar(ull_t frameIndex, ull_t numFrames, ull_t totalBars)
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

    cout << "] " << frameIndex << '/' << numFrames 
         << " (" << fixed << setprecision(3) << 100.*frameIndex/numFrames << ")% "
         << (frameIndex == numFrames ? ' ' : loadSeq[(loadSeqInd++) % loadSeq.size()]);
    cout.flush();
}

shared_ptr<Movie> Render2d::RenderAll()
{
    if (m_spScene->GetTimeSeq().empty())
    {
        throw runtime_error("Movie render attempted on scene with no time sequence! Did you use the correct scene constructor?");
    }

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
        PrintBar(aFrameIndex, numFrames, NUMBARS);
        this_thread::sleep_for(chrono::milliseconds(300));
    }

    for (auto &rt : renderThreads)
    {
        rt.join();
    }

    PrintBar(numFrames, numFrames, NUMBARS);

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
