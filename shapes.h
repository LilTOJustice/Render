#pragma once

#include "linalg.h"
#include "color.h"

class Solid
{

};

class Sprite
{
    public:
        Sprite(unsigned long long width, unsigned long long height)
            : m_width{width}, m_height{height}, m_pPixMap{new Color[height*width]}
        {}

        unsigned long long GetWidth();
        unsigned long long GetHeight();
        Color* GetPixMap();

        ~Sprite();

    private:
        unsigned long long m_width, m_height;
        Color* m_pPixMap;
};
