#pragma once
#include <vector>

//Entity system used by unity, unreal andin some sort by godot

class Component;

class Entity
{
    public:
        void Update();
        void Render();
        
        void AddComponent(Component *component);
        
    private:
        std::vector<Component*> components;
};

class Component 
{
    public:
        Component(Entity* entity) {this->entity = entity;}
        
        virtual void Update();
        virtual void Render();
    private:
        Entity *entity;
};


//TODO:implement
class EntityManager
{
    public:
        void AddEntities(Entity *enity);
        void RemoveEntities(Entity *entity);
        
        void Update();
        void Render();
        
    private:
        std::vector<Entity*> entities; 
};
