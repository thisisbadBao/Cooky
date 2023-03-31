#pragma once

#include <SDL2/SDL.h>

#include "../ECS/ECS.h"
#include "../Game/Game.h"
#include "../AssetManager/AssetManager.h"
#include "../Components/ScriptComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/TransformComponent.h"
#include "../Utils/CookyUtils.h"

void LuaBinding_AddTransform(Entity entity,
                             Vec2 position = Vec2::Zero,
                             Vec2 scale = Vec2(1,1),
                             double rotation = 0.0) {
    entity.AddComponent<TransformComponent>(position, scale, rotation);
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
}

void LuaBinding_AddRigidBody(Entity entity, Vec2 velocity = Vec2::Zero) {
    entity.AddComponent<RigidBodyComponent>(velocity);
}

void LuaBinding_AddText(Entity entity,
                        Vec2 position = Vec2::Zero,
                        std::string text = "",
                        std::string assetId = "",
                        Vec3 color = Vec3::Zero,
                        bool isFixed = true) {
    entity.AddComponent<TextLabelComponent>(position, text, assetId, color, isFixed);
}

void New_Usertype_Vec2(sol::state& lua) {
    lua.new_usertype<Vec2>(
        "Vec2",
        sol::constructors<Vec2(), Vec2(float x, float y)>(),
        "x", &Vec2::x,
        "y", &Vec2::y,
        sol::meta_function::addition, &Vec2::operator+,
        sol::meta_function::subtraction, &Vec2::operator-
    );
}

void New_Usertype_Vec3(sol::state& lua) {
    lua.new_usertype<Vec3>(
        "Vec3",
        sol::constructors<Vec3(), Vec3(float x, float y, float z)>(),
        "x", &Vec3::x,
        "y", &Vec3::y,
        "z", &Vec3::z,
        sol::meta_function::addition, &Vec3::operator+,
        sol::meta_function::subtraction, &Vec3::operator-
    );
}

void New_Usertype_Transform(sol::state& lua) {
    lua.new_usertype<TransformComponent>(
        "Transform",
        sol::constructors<TransformComponent(Vec2 position, Vec2 scale, double rotation)>(),
        "position", &TransformComponent::position,
        "scale", &TransformComponent::scale,
        "rotation", &TransformComponent::rotation
    );
}

void New_Usertype_Sprite(sol::state& lua) {
    lua.new_usertype<SpriteComponent>(
        "Sprite",
        sol::constructors<SpriteComponent(std::string assetIdm, int width, int height, int zIndex, bool isFixed, int srcRectX, int srcRectY)>(),
        "assetId", &SpriteComponent::assetId,
        "width", &SpriteComponent::width,
        "height", &SpriteComponent::height,
        "zIndex", &SpriteComponent::zIndex,
        "isFixed", &SpriteComponent::isFixed
    );
}

void New_Usertype_Text(sol::state& lua) {
    lua.new_usertype<TextLabelComponent>(
        "Text",
        sol::constructors<TextLabelComponent(Vec2 position , std::string text, std::string assetId, Vec3 color, bool isFixed)>(),
        "position", &TextLabelComponent::position,
        "text", &TextLabelComponent::text,
        "assetId", &TextLabelComponent::assetId,
        "color", &TextLabelComponent::color,
        "isFixed", &TextLabelComponent::isFixed
    );
}

void New_Usertype_Entity(sol::state& lua) {
    lua.new_usertype<Entity>(
        "entity",
        sol::constructors<Entity(int id), Entity(const Entity &entity)>(),
        "getId", &Entity::GetId,
        "kill", &Entity::Kill,
        "tag", &Entity::Tag,
        "hasTag", &Entity::HasTag,
        "group", &Entity::Group,
        "groupOf", &Entity::BelongsToGroup,
        "addTransform", LuaBinding_AddTransform,
        "removeTransform", &Entity::RemoveComponent<TransformComponent>,
        "hasTransform", &Entity::HasComponent<TransformComponent>,
        "getTransform", &Entity::GetComponent<TransformComponent>,
        "addSprite", LuaBinding_AddSprite,
        "removeSprite", &Entity::RemoveComponent<SpriteComponent>,
        "hasSprite", &Entity::HasComponent<SpriteComponent>,
        "getSprite", &Entity::GetComponent<SpriteComponent>,
        "addText", LuaBinding_AddText,
        "removeText", &Entity::RemoveComponent<TextLabelComponent>,
        "hasText", &Entity::HasComponent<TextLabelComponent>,
        "getText", &Entity::GetComponent<TextLabelComponent>
    );
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
        New_Usertype_Entity(lua);
        New_Usertype_Vec2(lua);
        New_Usertype_Vec3(lua);
        New_Usertype_Transform(lua);
        New_Usertype_Sprite(lua),
        New_Usertype_Text(lua),

        lua.set_function("test", Test);

        lua.set_function("createEntity", [&]() -> Entity { return registry->CreateEntity();});

        lua.set_function("addTexture", [&](std::string id, std::string file) {
            assetManager->AddTexture(id, file); 
            Logger::LogT("add texture: " + id + ", file: " + file);
        });

        lua.set_function("addFont", [&](std::string id, std::string file, int fontSize) {
            assetManager->AddFont(id, file, fontSize);
            Logger::LogT("add font: " + id);
        });
    }

    void Update() {
        for (auto entity: GetSystemEntities()) {
            const auto script = entity.GetComponent<ScriptComponent>();
            script.func(); // Invoke the function
        }
    }
};