#pragma once
#include <SDL2/SDL.h>
#include <ECS/ECS.hpp>
#include <ECS/Components/SpriteComponent.hpp>
#include <ECS/Components/AnimationComponent.hpp>

//TODO:isLoop flag doesn't do anything, change that
class AnimationSystem : public System
{
    public:
        AnimationSystem()
        {
            RequireComponent<Sprite_C>();
            RequireComponent<Animation_C>();
        }
        
        void Update()
        {
            for(auto entitiy: GetSystemEntities())
            {
                auto &sprite = entitiy.GetComponent<Sprite_C>();
                auto &animation = entitiy.GetComponent<Animation_C>();
                
                animation.currentFrame = ((SDL_GetTicks() - animation.startTicks)
                * animation.frameRateSpeed / 1000) % animation.numFrames;
                
                sprite.srcRect.x = animation.currentFrame * sprite.width;
                //sprite.srcRect.y = animation.currentFrame * sprite.height;
            }
        }
};
