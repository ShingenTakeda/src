#pragma once
#include <SDL2/SDL.h>

struct Animation_C
{
    int numFrames;
    int currentFrame;
    int frameRateSpeed;
    bool isLoop;
    int startTicks;
    
    Animation_C(int numFrames = 1, int frameRateSpeed = 1, bool isLoop = false)
    {
        this->numFrames = numFrames;
        this->currentFrame = 1;
        this->frameRateSpeed = frameRateSpeed;
        this->isLoop = isLoop;
        startTicks = SDL_GetTicks();
    }
};
