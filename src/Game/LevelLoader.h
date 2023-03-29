#pragma once

#include <memory>
#include <SDL2/SDL.h>
#include <sol/sol.hpp>

#include "../ECS/ECS.h"
#include "../AssetManager/AssetManager.h"

class LevelLoader {
public:
    LevelLoader();
    ~LevelLoader();
    void LoadLevel(sol::state& lua, const std::unique_ptr<Registry>& registry, std::unique_ptr<AssetManager>& assetManager, SDL_Renderer* renderer, int levelNumber);
};