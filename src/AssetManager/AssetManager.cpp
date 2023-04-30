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

    gSound->stopAll();
    for (auto sfx : soundEffect) {
        delete sfx.second.source;
    }
    soundEffect.clear();

    for (auto stream : soundStream) {
        delete stream.second.source;
    }
    soundStream.clear();
}

void AssetManager::SetRenderer(SDL_Renderer *renderer) {
    this->renderer = renderer;
}

void AssetManager::AddTexture(const std::string& assetId, const std::string& filePath) {
    if (!renderer) {
        Logger::Err("Add texture: \" " + assetId + " \" failed because SDL renderer is nullptr.");
        return;
    }
    SDL_Surface* surface = IMG_Load(filePath.c_str());
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    textures.emplace(assetId, texture);
    Logger::LogT("Add new texture: " + assetId);
}

SDL_Texture* AssetManager::GetTexture(const std::string& assetId) {
    return textures[assetId];
}

void AssetManager::AddFont(const std::string& assetId,
                           const std::string& filePath,
                           int fontSize) {
    fonts.emplace(assetId, TTF_OpenFont(filePath.c_str(), fontSize));
    Logger::LogT("Add new font: " + assetId);
}

TTF_Font* AssetManager::GetFont(const std::string& assetId) {
    return fonts[assetId];
}

void AssetManager::InitSound() {
    gSound = std::make_unique<SoLoud::Soloud>();
    gSound->init();
}

void AssetManager::DeinitSound() {
    gSound->deinit();
}

void AssetManager::AddSoundEffect(const std::string &audioName, const std::string &fileName) {
    if (soundEffect.find(audioName) != soundEffect.end()) {
        delete soundEffect[audioName].source;
        soundEffect.erase(audioName);
    }
    SoLoud::Wav* sound = new SoLoud::Wav;
    int res = sound->load(fileName.c_str());
    if (res == 0) {
        Logger::LogD("Add sfx:" + fileName);
        SoundEffect sfx;
        sfx.source = sound;
        soundEffect.emplace(audioName, sfx);
    } else {
        Logger::Err("Error: load sound effect " + fileName);
        delete sound;
    }
}

void AssetManager::AddSoundStream(const std::string &audioName, const std::string &fileName) {
    if (soundStream.find(audioName) != soundStream.end()) {
        delete soundStream[audioName].source;
        soundStream.erase(audioName);
    }
    SoLoud::WavStream* stream = new SoLoud::WavStream;
    int res = stream->load(fileName.c_str());
    if (res == 0) {
        Logger::LogD("Add stream:" + fileName);
        SoundStream sfx;
        sfx.source = stream;
        soundStream.emplace(audioName, sfx);
    } else {
        Logger::Err("Error: load stream " + fileName);
        delete stream;
    }
}

void AssetManager::Play(const std::string &audioName, float volume) {
    if (soundEffect.find(audioName) != soundEffect.end()) {
        uint handle = gSound->play(*soundEffect[audioName].source, volume);
        soundEffect[audioName].volume = volume;
        soundEffect[audioName].handle = handle;
    } else {
        Logger::Err("Audio: '" + audioName + "' not found.");
    }
}

void AssetManager::Stop(const std::string &audioName) {
    if (soundEffect.find(audioName) != soundEffect.end()) {
        gSound->stop(soundEffect[audioName].handle);
    } else {
        Logger::Err("Audio: '" + audioName + "' not found.");
    }
}

void AssetManager::PlayStream(const std::string &audioName, float volume) {
    if (soundStream.find(audioName) != soundStream.end()) {
        if (soundStream[audioName].isPlaying) {
            Logger::Err("Audio: stream '" + audioName + "' is playing.");
            return;
        }
        uint handle = gSound->play(*soundStream[audioName].source, volume);
        soundStream[audioName].volume = volume;
        soundStream[audioName].handle = handle;
        soundStream[audioName].isPlaying = true;
    } else {
        Logger::Err("Audio: '" + audioName + "' not found.");
    }
}

void AssetManager::StopStream(const std::string &audioName) {
    if (soundStream.find(audioName) != soundStream.end()) {
        gSound->stop(soundStream[audioName].handle);
        soundStream[audioName].isPlaying = false;
    } else {
        Logger::Err("Audio: '" + audioName + "' not found.");
    }
}
