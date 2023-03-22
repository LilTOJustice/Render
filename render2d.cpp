#include "render2d.h"

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

shared_ptr<Frame> Render2d::render(ld_t time, bool verbose)
{
    auto output = make_shared<Frame>(m_xRes, m_yRes);
    auto& scene = *m_spScene;
    uVec2 screenRes(m_xRes, m_yRes);

    if (verbose)
    {
        cout << "\nBeginning actor render (" << scene.getActors().size() << " total)...\n";
    }

    auto start = chrono::high_resolution_clock::now(), end = start;
    for (ull_t i = 0; i < m_yRes; i++)
    {
        for (ull_t j = 0; j < m_xRes; j++)
        {
            size_t arrayLoc = i * output->getWidth() + j;
            RGBA bgColor{0, 0, 0, 255};
            bgColor.rgb = scene.getBgColor();
            (*output.get())[arrayLoc] = bgColor;

            Vec2 worldCoord = scene.ssTransform(screenRes, uVec2{j, i});

            for (const auto &actor : scene.getActors())
            {
                Vec2 bottomRight = actor.getPos() + Vec2{ll_t(actor.getSize().x / 2), -ll_t(actor.getSize().y / 2)};
                Vec2 topLeft = actor.getPos() + Vec2{-ll_t(actor.getSize().x / 2), ll_t(actor.getSize().y / 2)};

                if (worldCoord.x <= bottomRight.x
                        && worldCoord.x >= topLeft.x
                        && worldCoord.y >= bottomRight.y
                        && worldCoord.y <= topLeft.y)
                {
                    auto spSprite = actor.getSprite();
                    auto* pixMap = spSprite->getPixMap();
                    uVec2 pixMapSize{spSprite->getWidth(), spSprite->getHeight()};

                    uVec2 pixMapInd = ((worldCoord - topLeft) / (bottomRight - topLeft)) * uVec2{spSprite->getWidth() - 1, spSprite->getHeight() - 1};
                    RGBA color = pixMap[pixMapInd.y * spSprite->getWidth() + pixMapInd.x];

                    for (const FragShader &fs : spSprite->getShaderQueue())
                    {
                        fs(color, color, uVec2{pixMapInd.x, (spSprite->getHeight() - 1) - pixMapInd.y}, pixMapSize, time);
                    }

                    (*output.get())[arrayLoc] = AlphaBlend(color, (*output.get())[arrayLoc]);
                }
            }
        }
    }

    if (verbose)
    {
        end = chrono::high_resolution_clock::now();
        cout << "Done! (" << chrono::duration_cast<chrono::duration<double>>(end - start).count() << "s)\n";

        // Final screen-space shader pass
        cout << "\nBeginning screen-space shader pass...\n";
        start = chrono::high_resolution_clock::now();
    }


    for (const FragShader &fs : scene.getShaderQueue())
    {
        for (size_t i = 0; i < output->getHeight(); i++)
        {
            for (size_t j = 0; j < output->getWidth(); j++)
            {
                fs((*output.get())[i * output->getWidth() + j], (*output.get())[i * output->getWidth() + j], uVec2{j, (output->getHeight() - 1) - i}, screenRes, time);
            }
        }
    }

    if (verbose)
    {
        end = chrono::high_resolution_clock::now();
        cout << "Done! (" << chrono::duration_cast<chrono::duration<double>>(end - start).count()
            << "s)\n\nRender complete.\n";
    }

    return output;
}

shared_ptr<Scene2d> Render2d::getScene()
{
    return m_spScene;
}

void threadRender(Render2d *_this, shared_ptr<Movie> spMovie, atomic_ullong *aFrameIndex)
{
    ull_t numFrames = spMovie->getNumFrames();
    for (ull_t frameInd = (*aFrameIndex)++; frameInd < numFrames; frameInd = (*aFrameIndex)++)
    {
        spMovie->writeFrame(_this->render(_this->getScene()->getTimeSeq()[frameInd], false), frameInd);
    }
}

void printBar(ull_t frameIndex, ull_t numFrames, ull_t totalBars)
{
    static ull_t loadSeqInd = 0;
    ull_t numBars = (1. * frameIndex / numFrames) * totalBars;

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
         << " (" << fixed << setprecision(3) << 100.*frameIndex / numFrames << "%) "
         << (frameIndex == numFrames ? ' ' : loadSeq[(loadSeqInd++) % loadSeq.size()]);
    cout.flush();
}

shared_ptr<Movie> Render2d::renderAll()
{
    if (m_spScene->getTimeSeq().empty())
    {
        throw runtime_error("Movie render attempted on scene with no time sequence! Did you use the correct scene constructor?");
    }

    if (m_numThreads <= 0)
    {
        throw runtime_error("Invalid number of threads given (" + to_string(m_numThreads) + ")!");
    }

    auto spMovie = make_shared<Movie>(m_xRes, m_yRes, m_spScene->getFps(), m_spScene->getTimeSeq().size());
    ull_t numFrames = spMovie->getNumFrames();
    cout << "\nBeginning render " << '(' << m_numThreads << " thread" << (m_numThreads > 1 ? "s" : "") << "): "
        << spMovie->getWidth() << 'x' << spMovie->getHeight() << " @ " << spMovie->getFps() << " -> "
        << numFrames << " frames (" << spMovie->getDuration() << "s)\n";

    auto start = chrono::high_resolution_clock::now();
    atomic_ullong aFrameIndex(0);
    vector<thread> renderThreads;

    for (ull_t i = 0; i < m_numThreads; i++)
    {
        renderThreads.emplace_back(threadRender, this, spMovie, &aFrameIndex);
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

    auto end = chrono::high_resolution_clock::now();

    cout << "\nDone! (" << chrono::duration_cast<chrono::duration<double>>(end - start).count()
        << "s)\n\nRender complete.\n";

    return spMovie;
}


void Render2d::setNumThreads(ull_t numThreads)
{
    m_numThreads = numThreads;
}
