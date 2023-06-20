#include <EC/EC.hpp>

void Entity::Update()
{
    for(int i = 0; i < components.max_size(); i++)
    {
        components.at(i)->Update();
    }
}

void Entity::Render()
{
    for(int i = 0; i < components.max_size(); i++)
    {
        components.at(i)->Render();
    }
}


void Entity::AddComponent (Component *component)
{
    components.push_back(component);
}
