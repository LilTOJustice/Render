#include "frame.h"

#include <chrono>
#include <iostream>

using namespace std;
using namespace cimg_library;

// Frame
Frame::Frame(unsigned long long width, unsigned long long height)
    : m_width{width}, m_height{height}, m_colorStride{width*height}, m_size{m_colorStride*3},
    m_aspectRatio{(long double)width/(long double)height}, m_pImage{new unsigned char[m_size]}
{}

void Frame::Output(string filename) const
{
    string fullname = filename + ".png";
    cout << "\nExporting frame as image: " << fullname << "...\n";
    auto start = chrono::high_resolution_clock::now();
    CImg<unsigned char> out(m_pImage, m_width, m_height, 1, 3);
    out.save_png(fullname.c_str());
    cout << "Done! (" << chrono::duration_cast<chrono::duration<double>>(chrono::high_resolution_clock::now()-start).count() << "s)\n";
}

RGBRef Frame::operator[](unsigned long long index)
{
    return RGBRef{
        m_pImage[index], // r
        m_pImage[index + m_colorStride], // g
        m_pImage[index + m_colorStride + m_colorStride] // b
    };
}

unsigned long long Frame::GetHeight() const
{
    return m_height;
}

unsigned long long Frame::GetWidth() const
{
    return m_width;
}

long double Frame::GetAspect() const
{
    return m_aspectRatio;
}

Frame::~Frame()
{
    delete[] m_pImage;
}

// Movie
unsigned long long Movie::m_nextId = 0;
Movie::Movie(unsigned long long width, unsigned long long height, unsigned long long fps, unsigned long long numFrames)
    : m_width{width}, m_height{height}, m_fps{fps}, m_numFrames{numFrames}, m_movieId{m_nextId++}, m_colorStride{width*height},
    m_imgSize{m_colorStride*3}, m_aspectRatio{(long double)width/(long double)height}, m_duration{(long double)(numFrames)/fps}
{}

void Movie::Output(string filename) const
{
    string fullname = filename + ".mp4";
    cout << "\nExporting movie: " << fullname << "...\n";

    auto start = chrono::high_resolution_clock::now();

    string cmd = "ffmpeg -y -v -8 -framerate " + to_string(m_fps)
        + " -f image2 -i temp/temp-" + to_string(m_movieId) + "-%d.png -c h264 -pix_fmt yuv420p -b:v 32768k "
        + fullname;
    cimg::system(cmd.c_str());
    system(("rm temp/temp-" + to_string(m_movieId) + "-*").c_str());
    
    cout << "Done! (" << chrono::duration_cast<chrono::duration<double>>(chrono::high_resolution_clock::now()-start).count() << "s)\n";
}

void Movie::WriteFrame(shared_ptr<Frame> spFrame, unsigned long long frameIndex)
{
    if (frameIndex >= m_numFrames)
    {
        throw runtime_error("Invalid frame index recieved!");
    }

    string filename = "temp/temp-" + to_string(m_movieId) + "-" + to_string(frameIndex) + ".png";
    CImg<unsigned char>(spFrame->m_pImage, m_width, m_height, 1, 3).save_png(filename.c_str());
}

unsigned long long Movie::GetHeight() const
{
    return m_height;
}

unsigned long long Movie::GetWidth() const
{
    return m_width;
}

unsigned long long Movie::GetFps() const
{
    return m_fps;
}

unsigned long long Movie::GetNumFrames() const
{
    return m_numFrames;
}

long double Movie::GetAspect() const
{
    return m_aspectRatio;
}

long double Movie::GetDuration() const
{
    return m_duration;
}
