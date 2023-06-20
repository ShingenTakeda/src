#pragma once
#include <ECS/ECS.hpp>
#include <ECS/Components/BoxColliderComponent.hpp>
#include <ECS/Components/TransformComponent.hpp>
#include <Base/EventBus.hpp>
#include <Events/CollisionEvent.hpp>

class CollisionSystem : public System
{
    public:
        CollisionSystem()
        {
            RequireComponent<Transform_C>();
            RequireComponent<BoxCollider_C>();
        }
        
        void Update(std::unique_ptr<EventBus> &eventBus)
        {
            auto entities = GetSystemEntities();
                
            for(auto i = entities.begin(); i != entities.end(); i++)
            {
                Entity a = *i;
                auto aTrans = a.GetComponent<Transform_C>();
                auto aCollider = a.GetComponent<BoxCollider_C>();
                    
                for(auto j = i; j != entities.end(); j++)
                {
                    Entity b = *j;
                    
                    if(a == b)
                    {
                        //Bypass the same entity
                        continue;
                    }
                     
                    auto bTrans = b.GetComponent<Transform_C>();
                    auto bCollider = b.GetComponent<BoxCollider_C>();
                    
                    bool collisionHappened = CheckAABB(aTrans.position.x + aCollider.offset.x, aTrans.position.y + aCollider.offset.y, 
                                                       aCollider.width, aCollider.height,
                                                       bTrans.position.x + bCollider.offset.x, bTrans.position.y + bCollider.offset.y, 
                                                       bCollider.width, bCollider.height);
                    
                    if(collisionHappened)
                    {
                        Logger::Log("Entity " + std::to_string(a.GetID()) + " is colliding with " + std::to_string(b.GetID()));
                        //TODO:Emit event
                        eventBus->EmitEvent<CollisionEvent>(a, b);
                    }
                }
            }
        }
        
        bool CheckAABB(double aX, double aY, double aW, double aH,
            double bX, double bY, double bW, double bH)
        {
            return 
                (
                    aX < bX + bW &&
                    aX + aW  > bX &&
                    aY < bY + bH &&
                    aY + aH > bY
                );
        }
};
