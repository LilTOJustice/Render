#include <fstream>
#include <iostream>

#include "image.h"

using namespace std;

void Image::Output(string filename) const
{
    ofstream file(filename+".ppm");
    file << "P3" << endl << m_width << ' ' << m_height << endl << "255" << endl;

    for (unsigned long long i = 0; i < m_height; ++i)
    {
        for (unsigned long long j = 0; j < m_width; ++j)
        {
            const Color &c = m_pImage[i*m_width+j];
            file << (int)c.r << ' ' << (int)c.g << ' ' << (int)c.b << ' ';
        }
    }

    file.close();
}

Color& Image::operator[](unsigned long long index)
{
    return m_pImage[index];
}

Image::~Image()
{
    delete[] m_pImage;
}
