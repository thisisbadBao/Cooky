#ifndef SCRIPTSYSTEM_H
#define SCRIPTSYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/ScriptComponent.h"
#include "../Components/TransformComponent.h"

std::tuple<double, double> GetEntityPosition(Entity entity) {
    if (entity.HasComponent<TransformComponent>()) {
        const auto transform = entity.GetComponent<TransformComponent>();
        return std::make_tuple(transform.position.x, transform.position.y);
    } else {
        Logger::Err("Entity has no TransformComponent");
        return std::make_tuple(0.0, 0.0);
    }
}

void SetEntityPosition(Entity entity, double x, double y) {
    if (entity.HasComponent<TransformComponent>()) {
        auto &transform = entity.GetComponent<TransformComponent>();
        transform.position.x = x;
        Logger::Log("Entity has TransformComponent");
        transform.position.y = y;
    } else {
        Logger::Err("Entity has no TransformComponent");
    }
}

class ScriptSystem: public System {
public: 
    ScriptSystem() {
        RequireComponent<ScriptComponent>();
    }

    void CreateLuaBindings(sol::state& lua) {
        lua.new_usertype<Entity>(
            "entity",
            "get_id", &Entity::GetId,
            "destroy", &Entity::Kill,
            "has_tag", &Entity::HasTag,
            "belongs_to_group", &Entity::BelongsToGroup
        );
        lua.set_function("get_position", GetEntityPosition);
        lua.set_function("set_position", SetEntityPosition);
    }

    void Update() {
        for (auto entity: GetSystemEntities()) {
            const auto script = entity.GetComponent<ScriptComponent>();
            script.func(entity); // Invoke the function
        }
    }
};

#endif