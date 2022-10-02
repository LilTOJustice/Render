#pragma once

#include <string>

#include "color.h"

class Image
{
    public:
    Image(unsigned long long width, unsigned long long height)
        : m_width{width}, m_height{height}, m_pImage{new Color[width*height]}
    {}

    void Output(std::string filename) const;

    Color& operator[](unsigned long long index);

    ~Image();

    private:
    unsigned long long m_width, m_height;
    Color* m_pImage;
};
