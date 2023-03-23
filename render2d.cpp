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

// Render2d
Render2d::Render2d(ull_t xRes, ull_t yRes, const shared_ptr<Scene2d> &spScene, ull_t numThreads)
    : m_xRes{xRes}, m_yRes{yRes}, m_spScene{spScene}, m_numThreads{numThreads},
    m_sceneThinkFunc{SceneThink(){return;}}
{
    if (m_spScene == nullptr)
    {
        throw runtime_error("Invalid scene provided to renderer.");
    }

    if (xRes < 1 || yRes < 1)
    {
         throw runtime_error("Invalid image size.");
    }
}

shared_ptr<Frame> Render2d::render(ld_t time, bool verbose) const
{
    return render(*m_spScene, time, verbose);
}

shared_ptr<Frame> Render2d::render(const Render2d::SceneInstance &scene, ld_t time, bool verbose) const
{
    auto output = make_shared<Frame>(m_xRes, m_yRes);
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
            RGBA bgColor{scene.getBgColor(), 255};
            (*output.get())[arrayLoc] = bgColor;

            Vec2 worldCoord = scene.ssTransform(screenRes, uVec2{j, i});

            // TODO: Implement actor rotation
            for (const Scene2d::Actor &actor : scene.getActors())
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

                    (*output)[arrayLoc] = AlphaBlend(color, (*output)[arrayLoc]);
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
                fs((*output)[i * output->getWidth() + j], (*output)[i * output->getWidth() + j], uVec2{j, (output->getHeight() - 1) - i}, screenRes, time);
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

shared_ptr<Scene2d> Render2d::getScene() const
{
    return m_spScene;
}

void threadRender(const Render2d &renderer, const vector<Render2d::SceneInstance> &sceneInstances, const shared_ptr<Movie> &spMovie, atomic_ullong &aFrameIndex)
{
    ull_t numFrames = spMovie->getNumFrames();
    for (ull_t frameInd = aFrameIndex++; frameInd < numFrames; frameInd = aFrameIndex++)
    {
        spMovie->writeFrame(renderer.render(sceneInstances[frameInd], renderer.getScene()->getTimeSeq()[frameInd], false), frameInd);
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

shared_ptr<Movie> Render2d::renderAll() const
{
    if (m_spScene->getTimeSeq().empty())
    {
        throw runtime_error("Movie render attempted on scene with no time sequence. Did you use the correct scene constructor?");
    }

    if (m_numThreads <= 0)
    {
        throw runtime_error("Invalid number of threads given (" + to_string(m_numThreads) + ")!");
    }

    cout << "\nBeginning simulation... ";
    auto start = chrono::high_resolution_clock::now();

    vector<SceneInstance> sceneInstances;
    sceneInstances.reserve(m_spScene->getTimeSeq().size());
    for (ld_t time : m_spScene->getTimeSeq())
    {
        m_sceneThinkFunc(time, m_spScene->getDt());
        sceneInstances.emplace_back(*m_spScene);
    }
    
    auto end = chrono::high_resolution_clock::now();
    
    cout << "Done! (" << chrono::duration_cast<chrono::duration<double>>(end - start).count()
        << "s)\n";

    auto spMovie = make_shared<Movie>(m_xRes, m_yRes, m_spScene->getFps(), m_spScene->getTimeSeq().size());
    ull_t numFrames = spMovie->getNumFrames();
    cout << "\nBeginning render " << '(' << m_numThreads << " thread" << (m_numThreads > 1 ? "s" : "") << "): "
        << spMovie->getWidth() << 'x' << spMovie->getHeight() << " @ " << spMovie->getFps() << " -> "
        << numFrames << " frames (" << spMovie->getDuration() << "s)\n";

    start = chrono::high_resolution_clock::now();
    atomic_ullong aFrameIndex(0);
    vector<thread> renderThreads;

    for (ull_t i = 0; i < m_numThreads; i++)
    {
        renderThreads.emplace_back(threadRender, ref(*this), ref(sceneInstances), ref(spMovie), ref(aFrameIndex));
    }

    // Wait for completion
    while (aFrameIndex < numFrames)
    {
        printBar(aFrameIndex, numFrames, NUMBARS);
        this_thread::sleep_for(chrono::milliseconds(500));
    }

    for (auto &rt : renderThreads)
    {
        rt.join();
    }

    printBar(numFrames, numFrames, NUMBARS);

    end = chrono::high_resolution_clock::now();

    cout << "\nDone! (" << chrono::duration_cast<chrono::duration<double>>(end - start).count()
        << "s)\n\nRender complete.\n";

    return spMovie;
}


void Render2d::setNumThreads(ull_t numThreads)
{
    m_numThreads = numThreads;
}

void Render2d::bindThinkFunc(const SceneThinkFunc &stf)
{
    m_sceneThinkFunc = stf;
}

void Render2d::unbindThinkFunc()
{
    m_sceneThinkFunc = SceneThink(){return;};
}

// Render2d::SceneInstance
Render2d::SceneInstance::SceneInstance(const Scene2d &scene)
    : m_camera{scene.getCamera()}, m_actors{}, m_bgColor{scene.getBgColor()}, m_shaderQueue{scene.getShaderQueue()}
{
    m_actors.reserve(scene.getActors().size());
    for (const auto &spActor : scene.getActors())
    {
        m_actors.push_back(*spActor);
    }
}

Scene2d::Camera& Render2d::SceneInstance::getCamera()
{
    return m_camera;
}

const Scene2d::Camera& Render2d::SceneInstance::getCamera() const
{
    return m_camera;
}

const vector<Scene2d::Actor>& Render2d::SceneInstance::getActors() const
{
    return m_actors;
}

RGB Render2d::SceneInstance::getBgColor() const
{
    return m_bgColor;
}

const vector<FragShader>& Render2d::SceneInstance::getShaderQueue() const
{
    return m_shaderQueue;
}

Vec2 Render2d::SceneInstance::ssTransform(const uVec2 &screenSize, const uVec2 &pixCoord) const
{
    return (Vec2{ll_t(pixCoord.x - ll_t(screenSize.x / 2)),
            ll_t(ll_t(screenSize.y / 2) - pixCoord.y)} / m_camera.getZoom() + m_camera.getCenter()).rot(m_camera.getRot());
}
