#pragma once
#include <ECS/ECS.hpp>
#include <Base/Event.hpp>

class CollisionEvent : public Event
{
    public:
        Entity a, b;
        CollisionEvent(Entity _a, Entity _b) : a(_a), b(_b) {} 
};
