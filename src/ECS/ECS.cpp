#include "ECS.h"
#include "../Logger/Logger.h"

int Entity::GetId() const {
    return id;
}

void System::AddEntityToSystem(Entity entity) {
    entities.push_back(entity);
}

void System::RemoveEntityFromSystem(Entity entity) {
    entities.erase(std::remove_if(entities.begin(), entities.end(), [&entity](Entity other) {
        return entity == other;
    }), entities.end());
}

std::vector<Entity> System::GetSystemEntities() const {
    return entities;
}

const Signature& System::GetComponentSignature() const {
    return componentSignature;
}

template <typename TComponent>
void System::RequireComponent() {
    const auto componentId = Component<TComponent>::GetId();
    componentSignature.set(componentId);
}

Entity CreateEntity() {
    int entityId = numEntites++;
    if (entityId >= entityComponentSigmatures.size()) {
        entityComponentSigmatures.resize(entityId + 1);
    }
    Entity entity(entityId);
    entitiesToBeAdded.insert(entity);

    Logger::Log("Entity created with id = " + std::to_string(entityId));

    return entity;
}

void Registry::Update() {
     
}

template <typename T, typename ...TArgs>
void AddComponent(Entity entity, TArgs&& ...args) {
    const auto componentId = Component<T>::GetId();
    const auto entityId = entity.GetId();

    if (componentId >= componentPools.size()) {
        componentPools.resize(componentId + 1, nullptr);
    }

    // If no pool for T
    if (!componentPools[componentId]) {
        Pool<T>* newComponentPool = new Pool<T>();
        componentPools[componentId] = newComponentPool;
    }

    // Get the pool for component T
    Pool<T> *componentPool = Pool<T>(componentPools[componentId]);

    if (entityId >= componentPool->GetSize()) {
        componentPool->Resize(numEntites);
    }

    // Create a new component
    T newComponet(std::forward<TArgs>(args)...);

    // Add component to the entity
    componentPool->Set(entityId, newComponent);

    // To turn on the component for the entity
    entityComponentSigmatures[entityId].set(componentId);
}

template <typename T>
void RemoveComponent(Entity entity){

}

template <typename T>
bool HasComponent(Entity entity) const {

}

template <typename T>
T& GetComponent(Entity entity) const {

}