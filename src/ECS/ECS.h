#ifndef ECS_H
#define ECS_H

#include <bitset>
#include <vector>
#include <unordered_map>
#include <typeindex>
#include <set>

const unsigned int MAX_COMPONENTS = 32;

// keep track of which componnets an entity has,
// and which entities a system is interested in.
typedef std::bitset<MAX_COMPONENTS> Signature;

struct IComponent {
    protected:
        static int nextId;
};

template <typename T>
class Component: public IComponent {
    // Return a unique id of a component<T>
    static int GetId() {
        static auto id = nextId++;
        return id;
    }
};

class Entity {
private:
    int id;

public:
    Entity(int id): id(id) {};
    Entity(const Entity &entity) = default;
    int GetId() const;

    Entity& operator=(const Entity& other) = default;
    bool operator==(const Entity& other) const { return other.GetId() == id; }
    bool operator!=(const Entity& other) const { return other.GetId() != id; }
    bool operator>(const Entity& other) const { return other.GetId() > id; }
    bool operator<(const Entity& other) const { return other.GetId() < id; }
};

// The system processes entities that contain a specific signature
class System {
private:
    Signature componentSignature;
    std::vector<Entity> entities;

public:
    System() = default;
    ~System() = default;

    void AddEntityToSystem(Entity entity);
    void RemoveEntityFromSystem(Entity entity);
    std::vector<Entity> GetSystemEntities() const;
    const Signature& GetComponentSignature() const;

    // The component type that entities must have to be considered by the system
    template <typename TComponent> void RequireComponent();
};

class IPool {
public:
    virtual ~IPool() {}
};

// A vector of objects of type T
template <typename T>
class Pool: public IPool {
private:
    std::vector<T> data;

public:
    Pool(int size = 100) { Resize(size); }
    virtual ~Pool() = default;

    bool isEmpty() const { return data.empty(); }
    int GetSize() const { return data.size(); }
    void Resize(int n) { data.resize(n); }
    void Clear() { data.clear(); }
    void Add(T object) { data.push_back(object); }
    void Set(int index, T object) { data[index] = object; }
    T& Get(int index) { return static_cast<T &>(data[index]); }
    T& operator[](unsigned int index) { return data[index]; }
};

// Manage the creation and destruction of entities, add systems, and components
class Registry {
private:
    int numEntites = 0;
    std::set<Entity> entitiesToBeAdded; // Entities will be created in the next Update() in Registry
    std::set<Entity> entitiesToBeKilled; // Entities will be killed in the next Update() in Registry

    // Each pool contains all the data of a certain component
    // vector index = component type id, Ipool index = entity id
    std::vector<IPool*> componentPools;

    // which component is "on" of each entity
    // vector index = entity id
    std::vector<Signature> entityComponentSigmatures;

    // Map of active system
    std::unordered_map<std::type_index, System*> systems;

public:
    Registry() = default;
    void Update();

    Entity CreateEntity();
    template <typename TComponent, typename ...TArgs> void AddComponent(Entity entity, TArgs&& ...args);
    template <typename T> void RemoveComponent(Entity entity);
    template <typename T> bool HasComponent(Entity entity) const;
    template <typename T> T& GetComponent(Entity entity) const;


    // void AddEntityToSystem(Entity entity);

    // Todo:
    // KillEntity()
    //
    // AddSystem()
    // RemoveSystem()
    // HasSystem()
    // GetSystem()
};

template <typename TComponent>
void System::RequireComponent() {
    const auto componentId = Component<TComponent>::GetId();
    componentSignature.set(componentId);
}

template <typename TComponent, typename ...TArgs>
void Registry::AddComponent(Entity entity, TArgs&& ...args) {
    const auto componentId = Component<TComponent>::GetId();
    const auto entityId = entity.GetId();

    if (componentId >= componentPools.size()) {
        componentPools.resize(componentId + 1, nullptr);
    }

    // If no pool for TComponent
    if (!componentPools[componentId]) {
        Pool<TComponent>* newComponentPool = new Pool<TComponent>();
        componentPools[componentId] = newComponentPool;
    }

    // Get the pool for TComponent
    Pool<TComponent> *componentPool = Pool<TComponent>(componentPools[componentId]);

    if (entityId >= componentPool->GetSize()) {
        componentPool->Resize(numEntites);
    }

    // Create a new component
    TComponent newComponent(std::forward<TArgs>(args)...);

    // Add component to the entity
    componentPool->Set(entityId, newComponent);

    // To turn on the component for the entity
    entityComponentSigmatures[entityId].set(componentId);
}

template <typename T>
void Registry::RemoveComponent(Entity entity){

}

template <typename T>
bool Registry::HasComponent(Entity entity) const {

}

template <typename T>
T& Registry::GetComponent(Entity entity) const {

}

#endif