#pragma once

#include <memory>
#include <string>
#include <vector>

#define cimg_display 0
#include "CImg.h"

#include "linalg.h"

class Frame
{
    public:
    Frame(unsigned long long width, unsigned long long height);

    void Output(std::string filename) const;

    RGBRef operator[](unsigned long long index);

    unsigned long long GetWidth() const;

    unsigned long long GetHeight() const;

    long double GetAspect() const;

    ~Frame();

    private:
    unsigned long long m_width, m_height, m_colorStride, m_size;
    long double m_aspectRatio;
    unsigned char* m_pImage;

    friend class Movie;
};

class Movie
{
    public:
    Movie(unsigned long long width, unsigned long long height, unsigned long long fps, unsigned long long numFrames);

    void Output(std::string filename) const;

    void WriteFrame(std::shared_ptr<Frame> spFrame, unsigned long long frameIndex);

    unsigned long long GetWidth() const;

    unsigned long long GetHeight() const;

    unsigned long long GetFps() const;

    unsigned long long GetNumFrames() const;

    long double GetAspect() const;

    long double GetDuration() const;

    private:
    unsigned long long m_width, m_height, m_fps, m_numFrames, m_movieId, m_colorStride, m_imgSize;
    long double m_aspectRatio, m_duration;
    static unsigned long long m_nextId;
};
