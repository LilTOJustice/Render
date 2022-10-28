#include <fstream>

#include "frame.h"

#define cimg_display 0
#include "CImg.h"

#include <chrono>
#include <iostream>

using namespace std;
using namespace cimg_library;

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

unsigned long long Frame::GetHeight()
{
    return m_height;
}

unsigned long long Frame::GetWidth()
{
    return m_width;
}

long double Frame::GetAspect()
{
    return m_aspectRatio;
}

Frame::~Frame()
{
    delete[] m_pImage;
}
