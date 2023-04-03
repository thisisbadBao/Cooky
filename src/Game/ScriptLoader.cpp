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

void ScriptLoader::LoadScript(sol::state& lua, const std::unique_ptr<Registry>& registry, std::unique_ptr<AssetManager>& assetManager) {
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

    // Read definitions of assets and entities from script
    sol::optional<sol::table> hasDef = lua["Def"];
    if (hasDef == sol::nullopt) {
        return;
    }
    sol::table def = lua["Def"];

    int i = 0;

    sol::optional<sol::table> hasAssets = def["assets"];
    if (hasAssets != sol::nullopt) {
        sol::table assets = def["assets"];
        while (true) {
            sol::optional<sol::table> hasAsset = assets[i];
            if (hasAsset == sol::nullopt) {
                break;
            }
            sol::table asset = assets[i];
            std::string assetType = asset["type"];
            std::string assetId = asset["id"];
            if (assetType == "texture") {
                assetManager->AddTexture(assetId, asset["file"]);
            }
            if (assetType == "font") {
                assetManager->AddFont(assetId, asset["file"], asset["font_size"]);
            }
            i++;
        }
    }
    

    sol::optional<sol::table> hasEntities = def["entities"];
    if (hasEntities == sol::nullopt) {
        return;
    }
    sol::table entities = def["entities"];
    i = 0;
    while (true) {
        sol::optional<sol::table> hasEntity = entities[i];
        if (hasEntity == sol::nullopt) {
            break;
        }

        sol::table entity = entities[i];

        Entity newEntity = registry->CreateEntity();

        // Tag
        sol::optional<std::string> tag = entity["tag"];
        if (tag != sol::nullopt) {
            newEntity.Tag(entity["tag"]);
        }

        // Group
        sol::optional<std::string> group = entity["group"];
        if (group != sol::nullopt) {
            newEntity.Group(entity["group"]);
        }

        // Components
        sol::optional<sol::table> hasComponents = entity["components"];
        if (hasComponents != sol::nullopt) {
            // Transform
            sol::optional<sol::table> transform = entity["components"]["transform"];
            if (transform != sol::nullopt) {
                newEntity.AddComponent<TransformComponent>(
                    Vec2(
                        entity["components"]["transform"]["position"]["x"],
                        entity["components"]["transform"]["position"]["y"]
                    ),
                    Vec2(
                        entity["components"]["transform"]["scale"]["x"].get_or(1.0),
                        entity["components"]["transform"]["scale"]["y"].get_or(1.0)
                    ),
                    entity["components"]["transform"]["rotation"].get_or(0.0)
                );
            }

            // RigidBody
            sol::optional<sol::table> rigidbody = entity["components"]["rigidbody"];
            if (rigidbody != sol::nullopt) {
                newEntity.AddComponent<RigidBodyComponent>(
                    Vec2(
                        entity["components"]["rigidbody"]["velocity"]["x"].get_or(0.0),
                        entity["components"]["rigidbody"]["velocity"]["y"].get_or(0.0)
                    )
                );
            }

            // Sprite
            sol::optional<sol::table> sprite = entity["components"]["sprite"];
            if (sprite != sol::nullopt) {
                newEntity.AddComponent<SpriteComponent>(
                    entity["components"]["sprite"]["texture_asset_id"],
                    entity["components"]["sprite"]["width"],
                    entity["components"]["sprite"]["height"],
                    entity["components"]["sprite"]["z_index"].get_or(1),
                    entity["components"]["sprite"]["fixed"].get_or(false),
                    Vec2(entity["components"]["sprite"]["src_rect"]["x"].get_or(0),
                         entity["components"]["sprite"]["src_rect"]["y"].get_or(0))
                );
            }

            // Animation
            sol::optional<sol::table> animation = entity["components"]["animation"];
            if (animation != sol::nullopt) {
                newEntity.AddComponent<AnimationComponent>(
                    entity["components"]["animation"]["num_frames"].get_or(1),
                    entity["components"]["animation"]["fps"].get_or(1),
                    entity["components"]["animation"]["isLoop"].get_or(false));
            }

            // BoxCollider
            sol::optional<sol::table> collider = entity["components"]["boxcollider"];
            if (collider != sol::nullopt) {
                newEntity.AddComponent<BoxColliderComponent>(
                    entity["components"]["boxcollider"]["width"],
                    entity["components"]["boxcollider"]["height"],
                    Vec2(
                        entity["components"]["boxcollider"]["offset"]["x"].get_or(0),
                        entity["components"]["boxcollider"]["offset"]["y"].get_or(0)
                    )
                );
            }

            // CameraFollow
            sol::optional<sol::table> cameraFollow = entity["components"]["camera_follow"];
            if (cameraFollow != sol::nullopt) {
                newEntity.AddComponent<CameraFollowComponent>();
            }

            // KeyboardControlled
            sol::optional<sol::table> keyboardControlled = entity["components"]["keyboard_controller"];
            if (keyboardControlled != sol::nullopt) {
                newEntity.AddComponent<KeyboardControlComponent>(
                    Vec2(
                        entity["components"]["keyboard_controller"]["up_velocity"]["x"],
                        entity["components"]["keyboard_controller"]["up_velocity"]["y"]
                    ),
                    Vec2(
                        entity["components"]["keyboard_controller"]["right_velocity"]["x"],
                        entity["components"]["keyboard_controller"]["right_velocity"]["y"]
                    ),
                    Vec2(
                        entity["components"]["keyboard_controller"]["down_velocity"]["x"],
                        entity["components"]["keyboard_controller"]["down_velocity"]["y"]
                    ),
                    Vec2(
                        entity["components"]["keyboard_controller"]["left_velocity"]["x"],
                        entity["components"]["keyboard_controller"]["left_velocity"]["y"]
                    )
                );
            }

            // Script
            sol::optional<sol::table> script = entity["components"]["update"];
            if (script != sol::nullopt) {
                sol::function func = entity["components"]["update"][0];
                newEntity.AddComponent<ScriptComponent>(func);
            }
        }
        i++;
    }
}