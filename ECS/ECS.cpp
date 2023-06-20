#include <ECS/ECS.hpp>
#include <algorithm>
#include <Logger/Logger.hpp>
#include <memory>

int IComponent::nextID = 0;

int Entity::GetID() const
{
    return m_id;
}

void Entity::Kill()
{
    world->KillEntity(*this);
}


//TODO:Check if entity already exist
void System::AddEntityToSystem ( Entity entity )
{
    m_entities.push_back(entity);
}

void System::RemoveEntityFromSystem ( Entity entity )
{
    m_entities.erase(std::remove_if(m_entities.begin(), m_entities.end(), 
        [&entity](Entity other)
        {
            return entity == other;
        }
    ),m_entities.end());
}

std::vector<Entity> System::GetSystemEntities() const
{
    return m_entities;
}

const Signature & System::GetComponentSignature() const
{
    //TODO:Drops const, could be better
    return m_componentSignature;
}

Entity World::CreateEntity()
{
    uint64_t entityID;
    
    if(m_freeIDS.empty())
    {
        //If there are no  free IDs, then we create new IDs and resize the signatures
        entityID = m_numEntities++;
        
        if(entityID >= m_entityComponentSignatures.size())
        {
            m_entityComponentSignatures.resize(entityID + 1);
        }
    }
    else
    {
        entityID = m_freeIDS.front();
        m_freeIDS.pop_front();
    }
    
    Entity entity(entityID);
    entity.world = this;
    m_entitiesToBeAdded.insert(entity);

    Logger::Log("Entity created with ID: " + std::to_string(entityID));
    
    return entity;
}

void World::KillEntity(Entity entity)
{
    m_entitiesToBeRemoved.insert(entity);
    Logger::Log("Killed entity with ID: " + std::to_string(entity.GetID()));
}

void World::AddEntityToSystem(Entity entity)
{
    const auto entityID = entity.GetID();
    const auto& componentSignature = m_entityComponentSignatures[entityID];
    
    for(auto& system : m_systems)
    {
        const auto& systemComponentSignature = system.second->GetComponentSignature();
        
        bool isInterested = (componentSignature & systemComponentSignature) == systemComponentSignature;
        if(isInterested)
        {
            system.second->AddEntityToSystem(entity);
        }
    }
}

void World::RemoveEntityFromSystem(Entity entity)
{
    for(auto systems : m_systems)
    {
        systems.second->RemoveEntityFromSystem(entity);
    }
}

void World::Update()
{
    for(auto entity : m_entitiesToBeAdded)
    {
        AddEntityToSystem(entity);
    }
    
    m_entitiesToBeAdded.clear();
    
    for(auto entity : m_entitiesToBeRemoved)
    {
        RemoveEntityFromSystem(entity);
        m_entityComponentSignatures[entity.GetID()].reset();//Make ID avaible for reuse
        m_freeIDS.push_back(entity.GetID());
    }
    
    m_entitiesToBeRemoved.clear();
}
