#ifndef LEVELLOADER_H
#define LEVELLOADER_H

#include <memory>
#include <SDL2/SDL.h>
#include <sol/sol.hpp>

#include "../ECS/ECS.h"
#include "../AssetStore/AssetStore.h"

class LevelLoader {
public:
    LevelLoader();
    ~LevelLoader();
    void LoadLevel(sol::state& lua, const std::unique_ptr<Registry>& registry, std::unique_ptr<AssetStore>& assetStore, SDL_Renderer* renderer, int levelNumber);
};

#endif