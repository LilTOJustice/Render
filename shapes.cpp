#include "shapes.h"

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
