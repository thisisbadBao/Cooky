#pragma once

#include "../Utils/CookyUtils.h"
#include <SDL2/SDL.h>

#include "../ECS/ECS.h"
#include "../Game/Game.h"
#include "../AssetManager/AssetManager.h"
#include "../Components/ScriptComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/TransformComponent.h"

void LuaBinding_AddTransform(Entity entity,
                             Vec2 position = Vec2(0,0),
                             Vec2 scale = Vec2(1,1),
                             double rotation = 0.0) {
    entity.AddComponent<TransformComponent>(position, scale, rotation);
    Logger::LogT("Add transform component");
}

void LuaBinding_AddSprite(Entity entity,
                          const std::string& assetId = "",
                          int width = 0,
                          int height = 0,
                          int zIndex = 0,
                          bool isFixed = false,
                          int srcRectX = 0,
                          int srcRectY = 0) {
    entity.AddComponent<SpriteComponent>(assetId, width, height, zIndex, isFixed, srcRectX, srcRectY);
    Logger::LogT("Add sprite component");
}

void LuaBinding_AddRigidBody(Entity entity, Vec2 velocity = Vec2(0.0, 0.0)) {
    entity.AddComponent<RigidBodyComponent>(velocity);
}

void Test() {
    Logger::LogT("Cooky v0.0.1");
}

class ScriptSystem: public System {
public: 
    ScriptSystem() {
        RequireComponent<ScriptComponent>();
    }

    void CreateLuaBindings(sol::state& lua, std::unique_ptr<Registry>& registry, std::unique_ptr<AssetManager>& assetManager,
        SDL_Renderer* renderer) {
        lua.new_usertype<Entity>(
            "entity",
            sol::constructors<Entity(int id), Entity(const Entity &entity)>(),
            "getId", &Entity::GetId,
            "kill", &Entity::Kill,
            "tag", &Entity::Tag,
            "hasTag", &Entity::HasTag,
            "group", &Entity::Group,
            "groupOf", &Entity::BelongsToGroup,
            "addTransform", &Entity::AddComponent<TransformComponent>,
            "removeTransform", &Entity::RemoveComponent<TransformComponent>,
            "hasTransform", &Entity::HasComponent<TransformComponent>,

            "addSprite", &Entity::AddComponent<SpriteComponent>,
            "addRigidBody", LuaBinding_AddRigidBody
        );
        lua.set_function("test", Test);
        lua.set_function("createEntity", [&]() -> Entity { return registry->CreateEntity();});
        lua.set_function("addTexture", [&](std::string id, std::string file) {
            assetManager->AddTexture(renderer, id, file); 
            Logger::LogT("add texture: " + id);
        });
        lua.set_function("addFont", [&](std::string id, std::string file, int fontSize) {
            assetManager->AddFont(id, file, fontSize);
            Logger::LogT("add font: " + id);
        });

    }

    void Update() {
        for (auto entity: GetSystemEntities()) {
            const auto script = entity.GetComponent<ScriptComponent>();
            script.func(entity); // Invoke the function
        }
    }
};