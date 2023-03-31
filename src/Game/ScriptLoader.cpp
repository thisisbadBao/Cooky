#include "ScriptLoader.h"

#include <fstream>

#include "./Game.h"
#include "../Components/TransformComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/AnimationComponent.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/KeyboardControlComponent.h"
#include "../Components/CameraFollowComponent.h"
#include "../Components/TextLabelComponent.h"
#include "../Components/ScriptComponent.h"

ScriptLoader::ScriptLoader() {
    Logger::LogD("ScriptLoader constructor called!");
}

ScriptLoader::~ScriptLoader() {
    Logger::LogD("ScriptLoader destructor called!");
}

void ScriptLoader::LoadScript(sol::state& lua, const std::unique_ptr<Registry>& registry, std::unique_ptr<AssetManager>& assetManager, SDL_Renderer* renderer,int levelNumber) {
    // syntax check
    sol::load_result script = lua.load_file("./assets/scripts/scriptTest.lua");
    if (!script.valid()) {
        sol::error err = script;
        std::string errMsg = err.what();
        Logger::Err("Error: " + errMsg);
        return;
    }

    lua.script_file("./assets/scripts/scriptTest.lua");

    sol::optional<sol::function> updateScript = lua["update"];
    if (updateScript != sol::nullopt) {
        sol::function update = lua["update"];
        Entity Update = registry->CreateEntity();
        Update.AddComponent<ScriptComponent>(update);
    }

    return;
}