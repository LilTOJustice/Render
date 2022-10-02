#include "shapes.h"

unsigned long long Sprite::GetWidth()
{
    return m_width;
}

unsigned long long Sprite::GetHeight()
{
    return m_height;
}

Color* Sprite::GetPixMap()
{
    return m_pPixMap;
}

Sprite::~Sprite()
{
    delete[] m_pPixMap;
}
