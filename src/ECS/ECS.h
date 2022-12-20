#ifndef ECS_H
#define ECS_H

#include <bitset>
#include <vector>

const unsigned int MAX_COMPONENTS = 32;

// keep track of which componnets an entity has,
// and which entities a system is interested in.
typedef std::bitset<MAX_COMPONENTS> Signature;

class Component
{
    // Todo:
};

class Entity
{
    private:
        int id;

    public:
        Entity(int id): id(id) {};
        int GetId() const;
        // ...
};

// The system processes entities that contain a specific signature
class System
{
    private:
        Signature componentSignature;
        std::vector<Entity> entities;

    public:
        System() = default;
        ~System() = default;

        void AddEntityToSystem(Entity entity);
        void RemoveEntityFromSystem(Entity entity);
        std::vector<Entity> GetSystemEntities() const;
        Signature& GetComponentSignature() const;
};

class Registry
{
    // Todo:
};

#endif