#pragma once

#include <SDL2/SDL.h>
#include <box2d/box2d.h>

#include "../ECS/ECS.h"
#include "../Game/Game.h"
#include "../Game/ScriptLoader.h"
#include "../AssetManager/AssetManager.h"
#include "../Components/ScriptComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/TextLabelComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../System/CallbackEventSystem.h"
#include "../System/PhysicsSystem.h"
#include "../System/KeyboardSystem.h"
#include "../Utils/CookyUtils.h"

void LuaBinding_AddTransform(Entity entity,
                             Vec2 position = Vec2::Zero) {
    entity.AddComponent<TransformComponent>(position, Vec2(1,1), 0);
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

void LuaBinding_AddRigidBody(Entity entity, Vec2 vel = Vec2(0.0, 0.0)) {
    entity.AddComponent<RigidBodyComponent>(vel);
}

void LuaBinding_AddText(Entity entity,
                        Vec2 position = Vec2::Zero,
                        std::string text = "",
                        std::string assetId = "",
                        Color color = Color::White,
                        bool isFixed = true) {
    entity.AddComponent<TextLabelComponent>(position, text, assetId, color, isFixed);
}

void LuaBinding_AddPolygon1(Entity& entity,
                            b2BodyDef bodyDef,
                            float width,
                            float height,
                            float friction,
                            float restitution,
                            float density) {
    entity.registry->GetSystem<PhysicsSystem>().AddPolygon(entity, bodyDef, width, height, friction, restitution, density);
}

void LuaBinding_AddPolygon2(Entity& entity,
                            b2BodyDef bodyDef,
                            std::vector<Vec2> vertices,
                            int count,
                            float friction,
                            float restitution,
                            float density) {
    b2Vec2 points[count];
    for (int i = 0; i < count; i++) {
        points[i].Set(vertices[i].x, vertices[i].y);
    }
    entity.registry->GetSystem<PhysicsSystem>().AddPolygon(entity, bodyDef, points, count, friction, restitution, density);
}

void LuaBinding_AddCircle(Entity& entity,
                          b2BodyDef bodyDef,
                          float radius,
                          float friction,
                          float restitution,
                          float density) {
    entity.registry->GetSystem<PhysicsSystem>().AddCircle(entity, bodyDef, radius, friction, restitution, density);
}

void LuaBinding_AddPhysicsFunctions(sol::state& lua, std::unique_ptr<Registry>& registry) {
    lua.set_function("SetLinearVelocity", [&registry](int entityId, Vec2 velocity) {
        registry->GetSystem<PhysicsSystem>().SetLinearVelocity(entityId, velocity);
    });

    lua.set_function("SetLinearDamping", [&registry](int entityId, float linearDamping) {
        registry->GetSystem<PhysicsSystem>().SetLinearDamping(entityId, linearDamping);
    });

    lua.set_function("SetAngularVelocity", [&registry](int entityId, float angularVelocity) {
        registry->GetSystem<PhysicsSystem>().SetAngularVelocity(entityId, angularVelocity);
    });

    lua.set_function("SetAngularDamping", [&registry](int entityId, float angularDamping) {
        registry->GetSystem<PhysicsSystem>().SetAngularDamping(entityId, angularDamping);
    });

    lua.set_function("ApplyForceToCenter", [&registry](int entityId, Vec2 force) {
        registry->GetSystem<PhysicsSystem>().ApplyForceToCenter(entityId, force);
    });

    lua.set_function("SetGravityScale", [&registry](int entityId, float scale) {
        registry->GetSystem<PhysicsSystem>().SetGravityScale(entityId, scale);
    });

    lua.set_function("SetSensor", [&registry](int entityId, bool sensor) {
        registry->GetSystem<PhysicsSystem>().SetSensor(entityId, sensor);
    });

    lua.set_function("OnContact", [&registry](int entityId, std::function<void(std::string)> callback) {
        registry->GetSystem<CollisionSystem>().SetBeginContactCallback(entityId, callback);
    });

    lua.set_function("EndContact", [&registry](int entityId, std::function<void(std::string)> callback) {
        registry->GetSystem<CollisionSystem>().SetEndContactCallback(entityId, callback);
    });
}

void LuaBinding_AddKeyboardFunctions(sol::state& lua, std::unique_ptr<Registry>& registry) {
    lua.set_function("IsKeyDown", [&registry](const std::string key) -> bool {
        return registry->GetSystem<KeyboardSystem>().IsKeyDown(key);
    });
}

void New_Usertype_Vec2(sol::state& lua) {
    lua.new_usertype<Vec2>(
        "Vec2", sol::call_constructor,
        sol::constructors<Vec2(), Vec2(float x, float y)>(),
        "x", &Vec2::x,
        "y", &Vec2::y,
        sol::meta_function::addition, &Vec2::operator+,
        sol::meta_function::subtraction, &Vec2::operator-
    );
}

void New_Usertype_b2Vec2(sol::state& lua) {
    lua.new_usertype<b2Vec2>(
        "b2Vec2", sol::call_constructor,
        sol::constructors<b2Vec2(), b2Vec2(float xIn, float yIn)>(),
        "x", &b2Vec2::x,
        "y", &b2Vec2::y
    );
}

void New_Usertype_Vec3(sol::state& lua) {
    lua.new_usertype<Vec3>(
        "Vec3", sol::call_constructor,
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
        "Color", sol::call_constructor,
        sol::constructors<Color(), Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a)>(),
        "r", &Color::r,
        "g", &Color::g,
        "b", &Color::b,
        "a", &Color::a
    );
}

void New_Usertype_Transform(sol::state& lua) {
    lua.new_usertype<TransformComponent>(
        "Transform", sol::call_constructor,
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
        "Text", sol::call_constructor,
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
        "RigidBody", sol::call_constructor,
        sol::constructors<RigidBodyComponent(Vec2 vel)>(),
        "vel", &RigidBodyComponent::vel
    );
}

void New_Usertype_BodyDef(sol::state& lua) {
    lua.new_usertype<b2BodyDef>(
        "BodyDef", sol::call_constructor,
        sol::constructors<b2BodyDef()>(),
        "position", &b2BodyDef::position,
        "angle", &b2BodyDef::angle,
        "linearVelocity", &b2BodyDef::linearVelocity,
        "angularVelocity", &b2BodyDef::angularVelocity,
        "linearDamping", &b2BodyDef::linearDamping,
        "angularDamping", &b2BodyDef::angularDamping,
        "allowSleep", &b2BodyDef::allowSleep,
        "awake", &b2BodyDef::awake,
        "fixedRotation", &b2BodyDef::fixedRotation,
        "bullet", &b2BodyDef::bullet,
        "type", &b2BodyDef::type,
        "enabled", &b2BodyDef::enabled,
        "gravityScale", &b2BodyDef::gravityScale
    );
}

void New_Enum_BodyType(sol::state& lua) {
    lua.new_enum<b2BodyType>("BodyType",{
        {"static", b2BodyType::b2_staticBody},
        {"kinematic", b2BodyType::b2_kinematicBody},
        {"dynamic", b2BodyType::b2_dynamicBody}
    });
}

void New_Usertype_Entity(sol::state& lua) {
    lua.new_usertype<Entity>(
        "entity", sol::call_constructor,
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
        "getRigidBody", &Entity::GetComponent<RigidBodyComponent>,
        "addPolygon", sol::overload(LuaBinding_AddPolygon1, LuaBinding_AddPolygon2),
        "addCircle", LuaBinding_AddCircle,
        "sub", &Entity::SubscribeEvent
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

    void CreateLuaBindings(sol::state& lua, std::unique_ptr<Registry>& registry, std::unique_ptr<AssetManager>& assetManager) {
        New_Usertype_Entity(lua);
        New_Usertype_b2Vec2(lua);
        New_Usertype_Vec2(lua);
        New_Usertype_Vec3(lua);
        New_Usertype_Color(lua);
        New_Usertype_Transform(lua);
        New_Usertype_Sprite(lua);
        New_Usertype_Text(lua);
        New_Usertype_RigidBody(lua);
        New_Usertype_BodyDef(lua);

        New_Enum_BodyType(lua);

        LuaBinding_AddPhysicsFunctions(lua, registry);
        LuaBinding_AddKeyboardFunctions(lua, registry);

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

        lua.set_function("emit", [&registry](std::string eventName) { registry->EmitEntityEvent(eventName); });

        lua.set_function("clog", [](const std::string& msg) { Logger::Log(msg); });

    }

    void Update() {
        for (auto entity: GetSystemEntities()) {
            const auto script = entity.GetComponent<ScriptComponent>();
            script.func(); // Invoke the function
        }
    }

    void UpdateScript(sol::state& _lua, std::unique_ptr<Registry>& _registry, std::unique_ptr<AssetManager>& _assetManager) {
        if (toBeReset) {
            _registry->Reset();
            _registry->GetSystem<PhysicsSystem>().Reset();
            _registry->GetSystem<CallbackEventSystem>().Reset();
            _registry->GetSystem<CollisionSystem>().Reset();
            ResetLuaState(_lua, _registry, _assetManager);

            ScriptLoader scriptLoader;
            for (auto script : scriptTobeReload) {
                Logger::LogT("Reload Script: " + script);
                scriptLoader.LoadScript(_lua, _registry, _assetManager, script);
            }
            scriptTobeReload.clear();

            toBeReset = false;
        }
    }

    void ResetLuaState(sol::state& _lua, std::unique_ptr<Registry>& _registry, std::unique_ptr<AssetManager>& _assetManager) {
        sol::state newLua;
        _lua = std::move(newLua);
        _lua.open_libraries(sol::lib::base, sol::lib::math, sol::lib::package, sol::lib::math);
        _registry->GetSystem<ScriptSystem>().CreateLuaBindings(_lua, _registry, _assetManager);
        _lua.script("package.path = package.path ..';./src/Lua/?.lua'");
        std::vector<std::string> scriptVec;
        ScriptLoader::GetScriptPath("./assets/scripts", scriptVec);
        for (auto script : scriptVec) {
            std::string packagePath = "package.path = package.path .. ';./assets/scripts/" + script + "/?.lua'";
            _lua.script(packagePath);
        }
    }

    void ResetScript() {
        toBeReset = true;
    }

    void AddScriptTobeReload(const std::string& scriptPath) {
        scriptTobeReload.emplace_back(scriptPath + "/main.lua");
        ResetScript();
    }

private:
    std::vector<std::string> scriptTobeReload;
    bool toBeReset = false;
};