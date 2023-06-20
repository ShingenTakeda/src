#pragma once
#include <memory>
#include <algorithm>
#include <SDL2/SDL.h>
#include <ECS/ECS.hpp>
#include <ECS/Components/SpriteComponent.hpp>
#include <ECS/Components/TransformComponent.hpp>
#include <Base/AssetManager.hpp>
#include <ECS/Components/SpriteComponent.hpp>

//TODO:Use this for debugDraw later(Create it with color support)!
//BUG:RenderSystem will render even without the proper components(sprite), maybe the bit is not being set? also, non deterministic.SOLVED
class RenderSystem : public System 
{
    public:
        RenderSystem()
        {
            RequireComponent<Transform_C>();
            RequireComponent<Sprite_C>();
        }
        
        void Render(SDL_Renderer *renderer, std::unique_ptr<AssetManager> &assetManager)
        {
            //TODO:Sort enities by zIndex(could be bad, could be good)
            struct Renderable
            {
                Transform_C tc;
                Sprite_C sc;
            };
                
            std::vector<Renderable> rv;
            for(auto entity : GetSystemEntities())
            {
                Renderable re;
                re.tc = entity.GetComponent<Transform_C>();
                re.sc = entity.GetComponent<Sprite_C>();
                
                rv.emplace_back(re);
            }
            
            std::sort(rv.begin(), rv.end(),
            [](const Renderable &a, const Renderable &b)
            {
                return a.sc.zDepth < b.sc.zDepth;
            });
            
            for(auto entity : rv)
            {
                const auto transform = entity.tc;
                const auto sprite = entity.sc;
                
                SDL_Rect srcRect = sprite.srcRect;
                
                //Newer versions of SDL2 have SDL_FloatRect, maybe use that instead
                SDL_Rect dstRect = {static_cast<int>(transform.position.x), 
                                    static_cast<int>(transform.position.y),
                                    static_cast<int>(sprite.width * transform.scale.x),
                                    static_cast<int>(sprite.height * transform.scale.y)};
                
                //SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);->for use in debugDraw
                //SDL_RenderFillRect(renderer, &entityRect);->for use in debugDraw
                SDL_RenderCopyEx(renderer, assetManager->GetTexture(sprite.textureID), &srcRect, &dstRect, 
                                transform.rotation, NULL, SDL_FLIP_NONE);
            }
        }
};
