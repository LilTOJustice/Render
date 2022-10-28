#pragma once

#include "linalg.h"

class Solid
{

};

class Sprite
{
    public:
        Sprite(unsigned long long width, unsigned long long height)
            : m_width{width}, m_height{height}, m_pPixMap{new RGBA[height*width]}
        {}

        unsigned long long GetWidth();
        unsigned long long GetHeight();
        RGBA* GetPixMap();

        ~Sprite();

    private:
        unsigned long long m_width, m_height;
        RGBA* m_pPixMap;
};
