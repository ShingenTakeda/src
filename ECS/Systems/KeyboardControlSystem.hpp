#pragma once
#include <SDL2/SDL.h>
#include <ECS/ECS.hpp>
#include <Base/EventBus.hpp>
#include <Events/KeyPressedEvent.hpp>
#include <ECS/Components/SpriteComponent.hpp>
#include <ECS/Components/RigidBodyComponent.hpp>
#include <Logger/Logger.hpp>

class KeyboardControlSystem : public System
{
    public:
        KeyboardControlSystem()
        {
            RequireComponent<RigidBody_C>();
            RequireComponent<Sprite_C>();
        }
        
        void SubscribeToEvent(std::unique_ptr<EventBus> &eventBus)
        {
            eventBus->SubscribeToEvent<KeyPressedEvent>(this, &KeyboardControlSystem::OnKeyPressed);
        }
        
        void OnKeyPressed(KeyPressedEvent &event)
        {
            std::string keyCode = std::to_string(event.symbol);
            std::string keySymbol(1, event.symbol);
            Logger::Log("Key pressed event emited: [" + keyCode + "] " + keySymbol);
        }
};
