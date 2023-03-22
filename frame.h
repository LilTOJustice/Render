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
    Frame(ull_t width, ull_t height);

    void Output(std::string filename) const;

    RGBARef operator[](ull_t index);

    ull_t GetWidth() const;

    ull_t GetHeight() const;

    ld_t GetAspect() const;

    ~Frame();

    private:
    ull_t m_width, m_height, m_colorStride, m_size;
    ld_t m_aspectRatio;
    byte_t* m_pImage;

    friend class Movie;
};

class Movie
{
    public:
    Movie(ull_t width, ull_t height, ull_t fps, ull_t numFrames);

    void Output(std::string filename) const;

    void WriteFrame(std::shared_ptr<Frame> spFrame, ull_t frameIndex);

    ull_t GetWidth() const;

    ull_t GetHeight() const;

    ull_t GetFps() const;

    ull_t GetNumFrames() const;

    ld_t GetAspect() const;

    ld_t GetDuration() const;

    ~Movie();

    private:
    ull_t m_width, m_height, m_fps, m_numFrames, m_movieId, m_colorStride, m_imgSize;
    ld_t m_aspectRatio, m_duration;
    std::string m_tempDir;

    static ull_t m_nextId;
};
