#pragma once

#include "linalg.h"

class Solid
{

};

class Sprite
{
    public:
        Sprite(ull_t width, ull_t height)
            : m_width{width}, m_height{height}, m_pPixMap{new RGBA[height*width]}
        {}

        ull_t GetWidth();
        ull_t GetHeight();
        RGBA* GetPixMap();

        ~Sprite();

    private:
        ull_t m_width, m_height;
        RGBA* m_pPixMap;
};
