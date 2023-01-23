#include "shapes.h"

#define cimg_display 0
#include "CImg.h"

#include <iostream> // XXX

using namespace std;
using namespace cimg_library;

Sprite::Sprite(ull_t width, ull_t height)
    : m_width{width}, m_height{height}, m_pPixMap{new RGBA[width*height]}
{}

Sprite::Sprite(const string filename)
{
    CImg<byte_t> img(filename.c_str());

    m_width = img.width();
    m_height = img.height();

    ull_t imgSize = m_width * m_height;
    ull_t spectrum = img.spectrum();
    m_pPixMap = new RGBA[m_width * m_height];
    
    for (ull_t i = 0; i < imgSize; i++)
    {
        if (spectrum == 1)
        {
            m_pPixMap[i].r = img[i];
            m_pPixMap[i].g = 255;
            m_pPixMap[i].b = 255;
            m_pPixMap[i].a = 255;
        }
        else if (spectrum == 2)
        {
            m_pPixMap[i].r = img[i];
            m_pPixMap[i].g = img[i + imgSize];
            m_pPixMap[i].b = 255;
            m_pPixMap[i].a = 255;
        }
        else if (spectrum == 3)
        {
            m_pPixMap[i].r = img[i];
            m_pPixMap[i].g = img[i + imgSize];
            m_pPixMap[i].b = img[i + 2 * imgSize];
            m_pPixMap[i].a = 255;
        }
        else
        {
            m_pPixMap[i].r = img[i];
            m_pPixMap[i].g = img[i + imgSize];
            m_pPixMap[i].b = img[i + 2 * imgSize];
            m_pPixMap[i].a = img[i + 3 * imgSize];
        }
    }
}

ull_t Sprite::GetWidth()
{
    return m_width;
}

ull_t Sprite::GetHeight()
{
    return m_height;
}

RGBA* Sprite::GetPixMap()
{
    return m_pPixMap;
}

Sprite::~Sprite()
{
    delete[] m_pPixMap;
}
