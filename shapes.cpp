#include "shapes.h"

#define cimg_display 0
#define cimg_use_png
#include "CImg.h"

#include <iostream>

using namespace std;
using namespace cimg_library;

Sprite::Sprite(ull_t width, ull_t height)
    : m_width{width}, m_height{height}, m_pPixMap{new RGBA[width * height]}
{}

Sprite::Sprite(const string filename)
{
    CImg<byte_t> img(filename.c_str());

    m_width = img.width();
    m_height = img.height();

    ull_t imgSize = m_width * m_height;
    ull_t spectrum = img.spectrum();
    m_pPixMap = new RGBA[m_width * m_height];

    switch(spectrum)
    {
        case 1:
            for (ull_t i = 0; i < imgSize; i++)
            {
                m_pPixMap[i].r = img[i];
                m_pPixMap[i].g = 255;
                m_pPixMap[i].b = 255;
                m_pPixMap[i].a = 255;    
            }
            break;

        case 2:
            for (ull_t i = 0; i < imgSize; i++)
            {
                m_pPixMap[i].r = img[i];
                m_pPixMap[i].g = img[i + imgSize];
                m_pPixMap[i].b = 255;
                m_pPixMap[i].a = 255;
            }
            break;

        case 3:
            for (ull_t i = 0; i < imgSize; i++)
            {
                m_pPixMap[i].r = img[i];
                m_pPixMap[i].g = img[i + imgSize];
                m_pPixMap[i].b = img[i + 2 * imgSize];
                m_pPixMap[i].a = 255;
            }
            break;

        default:
            for (ull_t i = 0; i < imgSize; i++)
            {
                m_pPixMap[i].r = img[i];
                m_pPixMap[i].g = img[i + imgSize];
                m_pPixMap[i].b = img[i + 2 * imgSize];
                m_pPixMap[i].a = img[i + 3 * imgSize];
            }
    }
}

ull_t Sprite::getWidth()
{
    return m_width;
}

ull_t Sprite::getHeight()
{
    return m_height;
}

const RGBA* Sprite::getPixMap()
{
    return m_pPixMap;
}

const vector<FragShader>& Sprite::getShaderQueue()
{
    return m_shaderQueue;
}
        
void Sprite::queueShader(const FragShader &fragShader)
{
    m_shaderQueue.push_back(fragShader);
}
void Sprite::clearShaders()
{
    m_shaderQueue.clear();
}

Sprite::~Sprite()
{
    delete[] m_pPixMap;
}
