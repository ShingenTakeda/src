#pragma once
#include <ECS/ECS.hpp>
#include <ECS/Components/RigidBodyComponent.hpp>
#include <ECS/Components/TransformComponent.hpp>
#include <Logger/Logger.hpp>

class MovementSystem : public System
{
    public:
        MovementSystem()
        {
            RequireComponent<Transform_C>();
            RequireComponent<RigidBody_C>();
        }
        
        void Update(double dt)
        {
            for(auto entity : GetSystemEntities())
            {
                auto &transform = entity.GetComponent<Transform_C>();
                const auto rigidbody = entity.GetComponent<RigidBody_C>();
                
                transform.position.x += rigidbody.velocity.x * dt;
                transform.position.y += rigidbody.velocity.y * dt;
            }
        }
};
