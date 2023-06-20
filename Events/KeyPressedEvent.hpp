#pragma once
#include <SDL2/SDL.h>
#include <ECS/ECS.hpp>
#include <Base/Event.hpp>


class KeyPressedEvent : public Event
{
    public:
        SDL_KeyCode symbol;
        KeypressedEvent(SDL_KeyCode _symbol) : symbol(_symbol) {}
};
