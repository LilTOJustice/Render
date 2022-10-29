#include <fstream>

#include "frame.h"

#define cimg_display 0
#include "CImg.h"

#include <chrono>
#include <iostream>

using namespace std;
using namespace cimg_library;

Frame::Frame(unsigned long long width, unsigned long long height)
    : m_width{width}, m_height{height}, m_colorStride{width*height}, m_size{m_colorStride*3},
    m_aspectRatio{(long double)width/(long double)height}, m_pImage{new unsigned char[m_size]}
{}

void Frame::Output(string filename) const
{
    string fullname = filename + ".png";
    cout << "Exporting frame as image: " << fullname << "...\n";
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

Movie::Movie(unsigned long long width, unsigned long long height, unsigned long long fps, unsigned long long numFrames)
    : m_width{width}, m_height{height}, m_fps{fps}, m_frameIndex{0}, m_colorStride{width*height}, m_imgSize{m_colorStride*3},
    m_aspectRatio{(long double)width/(long double)height}, m_duration{(long double)(numFrames)/fps}
{
    m_spFrames.resize(numFrames);
}

void Movie::Output(string filename) const
{
    string fullname = filename + ".mp4";
    cout << "Exporting movie: " << fullname << "...\n";
    auto start = chrono::high_resolution_clock::now();
    unsigned char *mov = new unsigned char[m_spFrames.size()*m_imgSize];
    for (size_t i = 0; i < m_spFrames.size(); i++)
    {
        memcpy(mov+(i*m_imgSize), m_spFrames[i]->m_pImage, m_imgSize);
        m_spFrames[i]->Output(to_string(i));
    }

    CImg<unsigned char> out(mov, m_width, m_height, m_spFrames.size(), 3);
    out.save_ffmpeg_external(fullname.c_str(), m_fps);
    cout << "Done! (" << chrono::duration_cast<chrono::duration<double>>(chrono::high_resolution_clock::now()-start).count() << "s)\n";
    delete[] mov;
}

shared_ptr<Frame> Movie::operator[](unsigned long long index)
{
    return m_spFrames.at(index);
}

void Movie::WriteFrame(shared_ptr<Frame> spFrame)
{
    if (m_frameIndex == m_spFrames.size())
    {
        throw runtime_error("Frame buffer is full!");
    }

    m_spFrames[m_frameIndex++] = spFrame;
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
    return m_spFrames.size();
}

long double Movie::GetAspect() const
{
    return m_aspectRatio;
}

long double Movie::GetDuration() const
{
    return m_duration;
}
