#pragma once
#include <glm/glm.hpp>

struct Transform_C
{
    glm::vec2 position;
    glm::vec2 scale;
    double rotation;
    
    Transform_C(glm::vec2 position = {0, 0}, glm::vec2 scale = {0, 0}, double rotation = 0)
    {
        this->position = position;
        this->scale = scale;
        this->rotation = rotation;
    }
};
