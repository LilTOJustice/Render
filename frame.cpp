#include "frame.h"

#include <chrono>
#include <filesystem>
#include <iostream>

using namespace std;
using namespace cimg_library;

// Frame
Frame::Frame(ull_t width, ull_t height)
    : m_width{width}, m_height{height}, m_colorStride{width*height}, m_size{m_colorStride*3},
    m_aspectRatio{(ld_t)width/(ld_t)height}, m_pImage{new byte_t[m_size]}
{}

void Frame::Output(string filename) const
{
    string fullname = filename + ".png";
    cout << "\nExporting frame as image: " << fullname << "...\n";
    auto start = chrono::high_resolution_clock::now();
    CImg<byte_t> out(m_pImage, m_width, m_height, 1, 3);
    out.save_png(fullname.c_str());
    cout << "Done! (" << chrono::duration_cast<chrono::duration<double>>(chrono::high_resolution_clock::now()-start).count() << "s)\n";
}

RGBRef Frame::operator[](ull_t index)
{
    return RGBRef{
        m_pImage[index], // r
        m_pImage[index + m_colorStride], // g
        m_pImage[index + m_colorStride + m_colorStride] // b
    };
}

ull_t Frame::GetHeight() const
{
    return m_height;
}

ull_t Frame::GetWidth() const
{
    return m_width;
}

ld_t Frame::GetAspect() const
{
    return m_aspectRatio;
}

Frame::~Frame()
{
    delete[] m_pImage;
}

// Movie
ull_t Movie::m_nextId = 0;
Movie::Movie(ull_t width, ull_t height, ull_t fps, ull_t numFrames)
    : m_width{width}, m_height{height}, m_fps{fps}, m_numFrames{numFrames}, m_movieId{m_nextId++}, m_colorStride{width*height},
    m_imgSize{m_colorStride*3}, m_aspectRatio{(ld_t)width/(ld_t)height}, m_duration{(ld_t)(numFrames)/fps}
{}

void Movie::Output(string filename) const
{
    filesystem::create_directory("temp");
    string fullname = filename + ".mp4";
    cout << "\nExporting movie: " << fullname << "...\n";

    auto start = chrono::high_resolution_clock::now();

    string cmd = "ffmpeg -y -v -8 -framerate " + to_string(m_fps)
        + " -f image2 -i temp/temp-" + to_string(m_movieId) + "-%d.png -c h264 -pix_fmt yuv420p -b:v 32768k "
        + fullname;
    cimg::system(cmd.c_str());
    cimg::system(("rm temp/temp-" + to_string(m_movieId) + "-*").c_str());
    
    cout << "Done! (" << chrono::duration_cast<chrono::duration<double>>(chrono::high_resolution_clock::now()-start).count() << "s)\n";
}

void Movie::WriteFrame(shared_ptr<Frame> spFrame, ull_t frameIndex)
{
    filesystem::create_directory("temp");
    if (frameIndex >= m_numFrames)
    {
        throw runtime_error("Invalid frame index recieved!");
    }

    string filename = "temp/temp-" + to_string(m_movieId) + "-" + to_string(frameIndex) + ".png";
    CImg<byte_t>(spFrame->m_pImage, m_width, m_height, 1, 3).save_png(filename.c_str());
}

ull_t Movie::GetHeight() const
{
    return m_height;
}

ull_t Movie::GetWidth() const
{
    return m_width;
}

ull_t Movie::GetFps() const
{
    return m_fps;
}

ull_t Movie::GetNumFrames() const
{
    return m_numFrames;
}

ld_t Movie::GetAspect() const
{
    return m_aspectRatio;
}

ld_t Movie::GetDuration() const
{
    return m_duration;
}
