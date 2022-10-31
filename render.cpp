#include "render.h"

#include <atomic>
#include <chrono>
#include <iostream>
#include <queue>
#include <stdexcept>
#include <thread>

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
        cout << "\nBeginning actor render (" << scene.GetActors().size() << " total)...\n";
    }

    auto start = chrono::high_resolution_clock::now();
    for (const auto &a : scene.GetActors())
    {
        auto spSprite = a.m_spSprite;
        //auto* pixMap = spSprite->GetPixMap();
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
        cout << "\nBeginning screen-space shader pass...\n";
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

struct jobinfo
{
    long double time;
    unsigned long long frameIndex;
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
    void QueueJob(long double time, unsigned long long frameIndex)
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
    atomic_ullong aFramesComplete(0);
    vector<WorkerThread> workerThreads(render_threads, WorkerThread(spMovie, this, aFramesComplete));

    for (size_t i = 0; i < spMovie->GetNumFrames(); i++)
    {
        workerThreads[i%render_threads].QueueJob(m_spScene->GetTimeSeq()[i], i);
    }

    for (auto &wt : workerThreads)
    {
        wt.Start();
    }

    for (auto &wt : workerThreads)
    {
        wt.Join();
    }

    cout << "Done! (" << chrono::duration_cast<chrono::duration<double>>(chrono::high_resolution_clock::now()-start).count() << "s)\n";
    return spMovie;
}

void Render2d::QueueShader(const FragShader& shader)
{
    m_shaderQueue.push(shader);
}
