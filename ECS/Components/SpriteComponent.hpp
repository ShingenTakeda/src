#pragma once
#include <cstdint>
#include <string>
#include <SDL2/SDL.h>

struct Sprite_C
{
    std::string textureID;
    int width;
    int height;
    uint8_t zDepth;//TODO:Call it layer 
    SDL_Rect srcRect;
    
    Sprite_C(std::string textureID = "", int width = 0, int height = 0, uint8_t zDepth = 0, int srcRectX = 0, int srcRctY = 0) 
    {
        this->width = width;
        this->height = height;
        this->zDepth = zDepth;
        this->textureID = textureID;
        this->srcRect = {srcRectX, srcRctY, width, height};
    }
};
