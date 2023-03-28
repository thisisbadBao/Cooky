#include "AssetManager.h"
#include <SDL2/SDL_image.h>
#include "../Logger/Logger.h"

AssetManager::AssetManager() {
    Logger::LogD("AssetManager constructor called!");
}

AssetManager::~AssetManager() {
    ClearAssets();
    Logger::LogD("AssetManager destructor called!");
}

void AssetManager::ClearAssets() {
    for (auto texture : textures) {
        SDL_DestroyTexture(texture.second);
    }
    textures.clear();

    for (auto font : fonts) {
        TTF_CloseFont(font.second);
    }
    fonts.clear();
}

void AssetManager::AddTexture(SDL_Renderer* renderer,
                              const std::string& assetId,
                              const std::string& filePath) {
    SDL_Surface* surface = IMG_Load(filePath.c_str());
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    textures.emplace(assetId, texture);
}

SDL_Texture* AssetManager::GetTexture(const std::string& assetId) {
    return textures[assetId];
}

void AssetManager::AddFont(const std::string& assetId,
                           const std::string& filePath,
                           int fontSize) {
    fonts.emplace(assetId, TTF_OpenFont(filePath.c_str(), fontSize));
}

TTF_Font* AssetManager::GetFont(const std::string& assetId) {
    return fonts[assetId];
}