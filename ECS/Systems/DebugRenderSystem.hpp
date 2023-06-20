#pragma once 
#include <SDL2/SDL.h>
#include <ECS/ECS.hpp>
#include <ECS/Components/TransformComponent.hpp>
#include <ECS/Components/BoxColliderComponent.hpp>

class DebugRenderSystem : public System
{
    public:
        DebugRenderSystem()
        {
            RequireComponent<Transform_C>();
            RequireComponent<BoxCollider_C>();
        }
        
        void Render(SDL_Renderer *renderer, bool shouldRender)
        {
            if(shouldRender)
            {
                for(auto entity : GetSystemEntities())
                {
                    const auto transform = entity.GetComponent<Transform_C>();
                    const auto collider = entity.GetComponent<BoxCollider_C>();
                    
                    SDL_Rect colliderRect = {static_cast<int>(transform.position.x + collider.offset.x),
                                            static_cast<int>(transform.position.y + collider.offset.y),
                                            static_cast<int>(collider.width), static_cast<int>(collider.height)};
                                            
                    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                    SDL_RenderDrawRect(renderer, &colliderRect);
                }
            }
        }
};
