#ifndef ECS_H
#define ECS_H

#include <bitset>
#include <vector>

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

class Registry {
    // Todo:
};

#endif