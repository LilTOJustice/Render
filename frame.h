#pragma once

#include <string>
#include "linalg.h"

class Frame
{
    public:
    Frame(unsigned long long width, unsigned long long height)
        : m_width{width}, m_height{height}, m_colorStride{width*height}, m_size{m_colorStride*3}, m_aspectRatio{(long double)width/(long double)height}, m_pImage{new unsigned char[m_size]}
    {}

    void Output(std::string filename) const;

    RGBRef operator[](unsigned long long index);

    unsigned long long GetWidth();

    unsigned long long GetHeight();

    long double GetAspect();

    ~Frame();

    private:
    unsigned long long m_width, m_height, m_colorStride, m_size;
    long double m_aspectRatio;
    unsigned char* m_pImage;
};
