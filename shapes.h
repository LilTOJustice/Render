#pragma once

#include <string>

#include "linalg.h"

class Solid
{

};

class Sprite
{
    public:
        Sprite(ull_t width, ull_t height);
        Sprite(const std::string filename);

        ull_t GetWidth();
        ull_t GetHeight();
        RGBA* GetPixMap();

        ~Sprite();

    private:
        ull_t m_width, m_height;
        RGBA* m_pPixMap;
};
