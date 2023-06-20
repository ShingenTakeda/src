#pragma once 
#include <glm/glm.hpp>

struct BoxCollider_C
{
    int width, height;
    glm::vec2 offset;
    
    BoxCollider_C(int _width = 0, int _height = 0, glm::vec2 _offset = glm::vec2(0)) : 
    width(_width), height(_height), offset(_offset)
    {
        
    }
};
