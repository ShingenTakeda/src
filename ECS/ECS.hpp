#pragma once
#include <cstdint>
#include <vector>
#include <bitset>
#include <unordered_map>
#include <typeindex>
#include <set>
#include <memory>
#include <deque>
#include <Logger/Logger.hpp>

//TODO:Change this ECS for a more cache friendly way

const uint64_t MAX_COMPONENTS = 1000;//Can later be changed
using Signature = std::bitset<MAX_COMPONENTS>;//Component signature

//Forward declarations
class Entity;
template<typename T> class Pool;
class IPool;
class World;

struct IComponent
{
    protected:
        static int nextID;
};

template <typename T>
class Component : public IComponent
{
    public:
        static int GetID()
        {
            static auto id = nextID++;
            return id;
        }
};

class System
{
    public:
        System() = default;
        virtual ~System() = default;
        
        void AddEntityToSystem(Entity entity);
        void RemoveEntityFromSystem(Entity entity);
        
        std::vector<Entity> GetSystemEntities() const;
        
        const Signature& GetComponentSignature() const;
        
        template<typename TComponent>
        void RequireComponent();
        
    private:
        Signature m_componentSignature;
        std::vector<Entity> m_entities;
};

class Entity
{
    public:
        Entity(int ID) : m_id(ID) {}
        Entity(const Entity &e) = default;
        
 
        Entity& operator =(const Entity& other) = default;
        bool operator ==(const Entity& other) const { return m_id == other.GetID(); }
        bool operator !=(const Entity& other) const { return m_id != other.GetID(); }
        bool operator >(const Entity& other) const { return m_id > other.GetID(); }
        bool operator <(const Entity& other) const { return m_id < other.GetID(); }
        
        template <typename Tcomponent, typename ... Targs> 
        void AddComponent(Targs && ... args);
        template <typename Tcomponent> 
        void RemoveComponent();
        template <typename Tcomponent>
        bool HasComponent() const;
        template <typename Tcomponent>
        Tcomponent &GetComponent() const;
        
        void Kill();
        
        int GetID() const;
        
        World *world;
    private:
        int m_id;
};

//ECS registry
class World
{
    public:
        World() {Logger::Log("World created!");}
        ~World() {Logger::Log("World destroyed!");}
        
        Entity CreateEntity();
        void KillEntity(Entity entity);
        
        //Component management
        template<typename Tcomponent, typename ...Targs>
        void AddComponent(Entity entity, Targs&& ...args);
        template<typename Tcomponent>
        void RemoveComponent(Entity entity);
        template<typename Tcomponent>
        Tcomponent& GetComponent(Entity entity) const;
        template<typename Tcomponent>
        bool HasComponent(Entity entity) const;
        
        //System management
        template<typename Tsystem>
        bool HasSystem() const;
        template<typename Tsystem, typename ... Targs>
        void AddSystem(Targs ... args);
        template<typename Tsystem>
        void RemoveSystem();
        template<typename Tsystem>
        Tsystem &GetSystem() const;
        
        void AddEntityToSystem(Entity entity);
        void RemoveEntityFromSystem(Entity entity);//TODO:implement
        
        void Update();
        
    private:
        int m_numEntities = 0;
        std::vector<std::shared_ptr<IPool>> m_componentPools;//Pool of components
        std::vector<Signature> m_entityComponentSignatures;//Keeps track which entity has which components
        std::unordered_map<std::type_index, std::shared_ptr<System>> m_systems;//System registry
        //Temp sets acting as buffers
        std::set<Entity> m_entitiesToBeAdded;
        std::set<Entity> m_entitiesToBeRemoved;//
        std::deque<int> m_freeIDS;//List of avaible free IDs
};

//Interface for pool
class IPool
{
    public:
        virtual ~IPool() {}
};

//Generic pool
template<typename T>
class Pool : public IPool
{
    public:
        Pool(int size = 100) {data.resize(size);}
        virtual ~Pool() = default;
        
        bool IsEmpty() const { return data.empty(); }
        int GetSize() const { return data.size(); }
        void Resize(int n) { data.resize(n); }
        void Clear() { data.clear(); }
        void Add(T object) { data.push_back(object); }
        void Set(int index, T object) { data[index] = object; }
        T &Get(int index) { return static_cast<T&>(data[index]); }
        T& operator[](unsigned int index) { return data[index]; }
        
    private:
        std::vector<T> data;
};

template<typename TComponent>
void System::RequireComponent()
{
    const auto componentID = Component<TComponent>::GetID();
    m_componentSignature.set(componentID);
}

template<typename Tcomponent, typename ...Targs> 
void World::AddComponent(Entity entity, Targs &&... args)
{
    const auto componentID = Component<Tcomponent>::GetID();
    const auto entityID = entity.GetID();
    
    if(componentID >= m_componentPools.size())
    {
        m_componentPools.resize(componentID + 1, nullptr);
    }
    
    if(!m_componentPools[componentID])
    {
        //std::shared_ptr<Pool<IComponent>> newComponentPool = std::make_shared<Pool<IComponent>>();
        std::shared_ptr<Pool<Tcomponent>> newComponentPool(new Pool<Tcomponent>());
        m_componentPools[componentID] = newComponentPool;
    }
    
    std::shared_ptr<Pool<Tcomponent>> componentPool = std::static_pointer_cast<Pool<Tcomponent>>(m_componentPools[componentID]);
    
    if(entityID >= componentPool->GetSize())
    {
        componentPool->Resize(m_numEntities);
    }
    
    Tcomponent newComponent(std::forward<Targs>(args)...);
    
    componentPool->Set(entityID, newComponent);
    m_entityComponentSignatures[entityID].set(componentID);
    
    Logger::Log("Component ID: " + std::to_string(componentID) + " was added to entity: " + std::to_string(entityID));
}

template<typename Tcomponent> 
bool World::HasComponent(Entity entity) const
{
    const auto componentID = Component<Tcomponent>::GetID();
    const auto entityID = entity.GetID();
    
    return m_entityComponentSignatures[entityID].test(componentID);
}

//TODO:Log if entity compoent is removed
template<typename Tcomponent> 
void World::RemoveComponent(Entity entity)
{
    const auto componentID = Component<Tcomponent>::GetID();
    const auto entityID = entity.GetID();
    
    m_entityComponentSignatures[entityID].set(componentID, false);
    Logger::Log("Removed component with ID: " + std::to_string(componentID) + " from entity: " + std::to_string(entityID));
}

template<typename Tsystem, typename ... Targs> 
void World::AddSystem(Targs... args)
{
    std::shared_ptr<Tsystem> newSystem = std::make_shared<Tsystem>(std::forward<Targs>(args) ...);
    m_systems.insert(std::make_pair(std::type_index(typeid(Tsystem)),newSystem));
}

template<typename Tsystem> 
void World::RemoveSystem()
{
    auto systemToRemove = m_systems.find(std::type_index(typeid(Tsystem)));
    m_systems.erase(systemToRemove);
}

template<typename Tsystem> 
bool World::HasSystem() const
{
    return m_systems.find(std::type_index(typeid(Tsystem))) !=  m_systems.end();
}

template<typename Tsystem> 
Tsystem & World::GetSystem() const
{
    auto system = m_systems.find(std::type_index(typeid(Tsystem)));
    return *(std::static_pointer_cast<Tsystem>(system->second));
}

template<typename Tcomponent> 
Tcomponent & World::GetComponent(Entity entity) const
{
    const auto componentID = Component<Tcomponent>::GetID();
    const auto entityID = entity.GetID();
    auto componentPool = std::static_pointer_cast<Pool<Tcomponent>>(m_componentPools[componentID]);
    
    return componentPool->Get(entityID);
}

template<typename Tcomponent, typename ... Targs> 
void Entity::AddComponent(Targs && ... args)
{
    world->AddComponent<Tcomponent>(*this, std::forward<Targs>(args)...);
}

template<typename Tcomponent>
void Entity::RemoveComponent()
{
    world->RemoveComponent<Tcomponent>(*this);
}

template<typename Tcomponent> 
bool Entity::HasComponent() const
{
    return world->HasComponent<Tcomponent>(*this);
}

template<typename Tcomponent> 
Tcomponent & Entity::GetComponent() const
{
    return world->GetComponent<Tcomponent>(*this);
}
