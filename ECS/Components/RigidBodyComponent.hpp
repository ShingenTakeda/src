#pragma once
#include <glm/glm.hpp>

struct RigidBody_C
{
    glm::vec2 velocity;
    
    RigidBody_C(glm::vec2 vel = glm::vec2(0.0, 0.0)) : velocity(vel) {}
};
