#pragma once

#include <string>
#include <vector>

#include "linalg.h"
#include "shader.h"

class Solid
{

};

class Sprite
{
    public:
        Sprite(ull_t width, ull_t height);
        Sprite(const std::string filename);

        ull_t getWidth();
        ull_t getHeight();
        const RGBA* getPixMap();
        const std::vector<FragShader>& getShaderQueue();
        void queueShader(const FragShader &fragShader);
        void clearShaders();

        ~Sprite();

    private:
        ull_t m_width, m_height;
        RGBA* m_pPixMap;
        std::vector<FragShader> m_shaderQueue;
};
