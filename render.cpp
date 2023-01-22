#include "render.h"

#include <atomic>
#include <chrono>
#include <iostream>
#include <stdexcept>
#include <thread>

using namespace std;

Render2d::Render2d(ull_t xRes, ull_t yRes, std::shared_ptr<Scene2d> spScene)
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
        cout << "\nBeginning screen-space shader pass...\n";
    }

    start = chrono::high_resolution_clock::now();
    size_t numShaders = m_shaderQueue.size();
    uVec2 screenRes(m_xRes, m_yRes);

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
                shader((*output.get())[i*output->GetWidth()+j], uVec2{j, (output->GetHeight()-1)-i}, screenRes, time);
            }
        }

        // Go to next shader
        m_shaderQueue.pop();
        m_shaderQueue.push(shader);
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

class WorkerThread
{
    static void Work(queue<jobinfo> *jobQueue, shared_ptr<Movie> spMovie, Render2d *pRenderer, atomic_ullong *aFramesComplete)
    {
        while(!jobQueue->empty())
        {
            const jobinfo &ji = jobQueue->front();
            spMovie->WriteFrame(pRenderer->Render(ji.time, false), ji.frameIndex);
            (*aFramesComplete)++;
            
            if (*aFramesComplete % 5 == 0 || *aFramesComplete == spMovie->GetNumFrames())
            {
                cout << 100.*(*aFramesComplete)/spMovie->GetNumFrames() << "%\n";
                
            }

            jobQueue->pop();
        }
    }

    public:
    void QueueJob(ld_t time, ull_t frameIndex)
    {
        m_jobQueue.push({time, frameIndex});
    }
    
    WorkerThread(shared_ptr<Movie> spMovie, Render2d *pRenderer, atomic_ullong &aFramesComplete)
        : m_spMovie{spMovie}, m_pRenderer{pRenderer}, m_aFramesComplete{aFramesComplete}
    {}

    void Start()
    {
        m_spThread = make_shared<thread>(Work, &m_jobQueue, m_spMovie, m_pRenderer, &m_aFramesComplete);
    }

    void Join()
    {
        m_spThread->join();
    }
    
    private:
    shared_ptr<thread> m_spThread;
    shared_ptr<Movie> m_spMovie;
    queue<jobinfo> m_jobQueue;
    Render2d *m_pRenderer;
    atomic_ullong &m_aFramesComplete;
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
        if (frameInd % 5 == 0 || frameInd == numFrames)
        {
            cout << 100.*frameInd/spMovie->GetNumFrames() << "%\n";
        }
    }
}

shared_ptr<Movie> Render2d::RenderAll()
{
    if (render_threads <= 0)
    {
        throw runtime_error("Invalid number of threads given (" + to_string(render_threads) + ")!");
    }

    auto spMovie = make_shared<Movie>(m_xRes, m_yRes, m_spScene->GetFps(), m_spScene->GetTimeSeq().size());
    cout << "\nBeginning render " << '(' << render_threads << " thread" << (render_threads > 1 ? "s" : "") << "): "
        << spMovie->GetWidth() << 'x' << spMovie->GetHeight() << " @ " << spMovie->GetFps() << " -> "
        << spMovie->GetNumFrames() << " frames (" << spMovie->GetDuration() << "s)\n";

    auto start = chrono::high_resolution_clock::now();
    atomic_ullong aFrameIndex(0);
    vector<thread> renderThreads;

    for (ull_t i = 0; i < render_threads; i++)
    {
        renderThreads.emplace_back(ThreadRender, this, spMovie, &aFrameIndex);
    }

    // Wait for completion
    for (auto &rt : renderThreads)
    {
        rt.join();
    }

    cout << "Done! (" << chrono::duration_cast<chrono::duration<double>>(chrono::high_resolution_clock::now()-start).count() << "s)\n";
    return spMovie;
}

void Render2d::QueueShader(const FragShader& shader)
{
    m_shaderQueue.push(shader);
}
