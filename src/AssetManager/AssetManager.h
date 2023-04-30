#pragma once

#include <map>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <soloud.h>
#include <soloud_wav.h>
#include <soloud_wavstream.h>

struct SoundEffect {
    SoLoud::Wav* source;
    uint handle = 0;
    float volume = 1.0f;
};

struct SoundStream {
    SoLoud::WavStream* source;
    uint handle = 0;
    float volume = 1.0f;
    bool isPlaying = false;
};

class AssetManager
{
private:
    std::map<std::string, SDL_Texture*> textures;
    std::map<std::string, TTF_Font*> fonts;
    std::map<std::string, SoundEffect> soundEffect;
    std::map<std::string, SoundStream> soundStream;
    SDL_Renderer* renderer;
    std::unique_ptr<SoLoud::Soloud> gSound;

public:
    AssetManager();
    ~AssetManager();

    void ClearAssets();

    void SetRenderer(SDL_Renderer *renderer);

    void AddTexture(const std::string& assetId, const std::string& filePath);
    SDL_Texture* GetTexture(const std::string& assetId);

    void AddFont(const std::string &assetId, const std::string &filePath, int fontSize);
    TTF_Font* GetFont(const std::string &assetId);

    void InitSound();
    void DeinitSound();
    void AddSoundEffect(const std::string &audioName, const std::string &fileName);
    void AddSoundStream(const std::string &audioName, const std::string &fileName);
    void Play(const std::string &audioName, float volume);
    void Stop(const std::string &audioName);
    void PlayStream(const std::string &audioName, float volume);
    void StopStream(const std::string &audioName);
};