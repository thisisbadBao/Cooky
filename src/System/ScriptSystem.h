#pragma once

#include <SDL2/SDL.h>

#include "../ECS/ECS.h"
#include "../Game/Game.h"
#include "../AssetManager/AssetManager.h"
#include "../Components/ScriptComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/TextLabelComponent.h"
#include "../Components/RigidBodyComponent.h"
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
                          Vec2 srcRect = Vec2::Zero) {
    entity.AddComponent<SpriteComponent>(assetId, width, height, zIndex, isFixed, srcRect);
}

void LuaBinding_AddRigidBody(Entity entity, Vec2 velocity = Vec2::Zero) {
    entity.AddComponent<RigidBodyComponent>(velocity);
}

void LuaBinding_AddText(Entity entity,
                        Vec2 position = Vec2::Zero,
                        std::string text = "",
                        std::string assetId = "",
                        Color color = Color::White,
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

void New_Usertype_Color(sol::state& lua) {
    lua.new_usertype<Color>(
        "Color",
        sol::constructors<Color(), Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a)>(),
        "r", &Color::r,
        "g", &Color::g,
        "b", &Color::b,
        "a", &Color::a
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
        sol::constructors<SpriteComponent(std::string assetIdm, int width, int height, int zIndex, bool isFixed, Vec2 srcRect)>(),
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
        sol::constructors<TextLabelComponent(Vec2 position, std::string text, std::string assetId, Color color, bool isFixed)>(),
        "position", &TextLabelComponent::position,
        "text", &TextLabelComponent::text,
        "assetId", &TextLabelComponent::assetId,
        "color", &TextLabelComponent::color,
        "isFixed", &TextLabelComponent::isFixed
    );
}

void New_Usertype_RigidBody(sol::state& lua) {
    lua.new_usertype<RigidBodyComponent>(
        "RigidBody",
        sol::constructors<RigidBodyComponent(Vec2 velocity)>(),
        "velocity", &RigidBodyComponent::velocity
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
        "setTransform", &Entity::SetComponentOn<TransformComponent>,
        "rmTransform", &Entity::RemoveComponent<TransformComponent>,
        "hasTransform", &Entity::HasComponent<TransformComponent>,
        "getTransform", &Entity::GetComponent<TransformComponent>,
        "addSprite", LuaBinding_AddSprite,
        "setSprite", &Entity::SetComponentOn<SpriteComponent>,
        "rmSprite", &Entity::RemoveComponent<SpriteComponent>,
        "hasSprite", &Entity::HasComponent<SpriteComponent>,
        "getSprite", &Entity::GetComponent<SpriteComponent>,
        "addText", LuaBinding_AddText,
        "setText", &Entity::SetComponentOn<TextLabelComponent>,
        "rmText", &Entity::RemoveComponent<TextLabelComponent>,
        "hasText", &Entity::HasComponent<TextLabelComponent>,
        "getText", &Entity::GetComponent<TextLabelComponent>,
        "addRigidBody", LuaBinding_AddRigidBody,
        "setRigidBody", &Entity::SetComponentOn<RigidBodyComponent>,
        "rmRigidBody", &Entity::RemoveComponent<RigidBodyComponent>,
        "hasRigidBody", &Entity::HasComponent<RigidBodyComponent>,
        "getRigidBody", &Entity::GetComponent<RigidBodyComponent>
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

    void CreateLuaBindings(sol::state& lua,std::unique_ptr<Registry>& registry, std::unique_ptr<AssetManager>& assetManager) {
        New_Usertype_Entity(lua);
        New_Usertype_Vec2(lua);
        New_Usertype_Vec3(lua);
        New_Usertype_Color(lua);
        New_Usertype_Transform(lua);
        New_Usertype_Sprite(lua);
        New_Usertype_Text(lua);
        New_Usertype_RigidBody(lua);

        lua.set_function("test", Test);

        lua.set_function("newEnt", [&registry]() -> Entity { return registry->CreateEntity();});

        lua.set_function("getEntByTag", [&registry](const std::string &tag) -> Entity {
            return registry->GetEntityByTag(tag); });

        lua.set_function("addTexture", [&assetManager](std::string id, std::string file) {
            assetManager->AddTexture(id, file);
        });

        lua.set_function("addFont", [&assetManager](std::string id, std::string file, int fontSize) {
            assetManager->AddFont(id, file, fontSize);
        });
    }

    void Update() {
        for (auto entity: GetSystemEntities()) {
            const auto script = entity.GetComponent<ScriptComponent>();
            script.func(); // Invoke the function
        }
    }

    void UpdateScript(sol::state& _lua, std::unique_ptr<Registry>& _registry, std::unique_ptr<AssetManager>& _assetManager) {
        for (auto script : scriptTobeReload) {
            Logger::LogT("Reload Script: " + script);
            Game::ReloadScript(_lua, _registry, _assetManager, script);
        }
        scriptTobeReload.clear();
        if (toBeReset) {
            _registry->Reset();
            toBeReset = false;
            ResetLuaState(_lua, _registry, _assetManager);
        }
    }

    void ResetLuaState(sol::state& _lua, std::unique_ptr<Registry>& _registry, std::unique_ptr<AssetManager>& _assetManager) {
        sol::state newLua;
        _lua = std::move(newLua);
        _lua.open_libraries(sol::lib::base, sol::lib::math);
        _registry->GetSystem<ScriptSystem>().CreateLuaBindings(_lua, _registry, _assetManager);
    }

    void ResetScript() {
        toBeReset = true;
    }

    void AddScriptTobeReload(const std::string& scriptPath) {
        scriptTobeReload.emplace_back(scriptPath);
    }

private:
    std::vector<std::string> scriptTobeReload;
    bool toBeReset = false;
};