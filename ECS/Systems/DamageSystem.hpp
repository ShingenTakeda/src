#pragma once
#include <Logger/Logger.hpp>
#include <ECS/ECS.hpp>
#include <ECS/Components/BoxColliderComponent.hpp>
#include <Base/EventBus.hpp>
#include <Events/CollisionEvent.hpp>

class DamageSystem : public System
{
    public:
        DamageSystem()
        {
            RequireComponent<BoxCollider_C>();
        }
        
        void SubscribeToEvent(std::unique_ptr<EventBus> &eventBus)
        {
            //TODO:Make a better Event system since templates cant actually figure this one out without ignoring warnings
            eventBus->SubscribeToEvent<CollisionEvent>(this, &DamageSystem::OnCollision);
        }
        
        void OnCollision(CollisionEvent &event)
        {
            Logger::Log("The DamageSystem received an event with a colission between entities: " + 
            std::to_string(event.a.GetID()) + " and " + std::to_string(event.b.GetID()));
            event.a.Kill();
            event.b.Kill();
        }

        void Update()
        {
            
        }
};
