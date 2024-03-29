#pragma once

#include "../ECS/ECS.h"
#include "../AssetManager/AssetManager.h"
#include "../Components/TextLabelComponent.h"
#include <SDL2/SDL.h>

class RenderTextSystem: public System {
public:
    RenderTextSystem()
    {
        RequireComponent<TextLabelComponent>();
    }

    void Update(SDL_Renderer* renderer, std::unique_ptr<AssetManager>& assetManager, SDL_Rect camera) {
        for (auto entity : GetSystemEntities()) {
            const auto textLabel = entity.GetComponent<TextLabelComponent>();
            const SDL_Color _color = {textLabel.color.r, textLabel.color.g, textLabel.color.b, textLabel.color.a};
            SDL_Surface *surface = TTF_RenderText_Blended(
                assetManager->GetFont(textLabel.assetId),
                textLabel.text.c_str(),
                _color
            );

            SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_FreeSurface(surface);

            int labelWidth = 0;
            int labelHeight = 0;
            SDL_QueryTexture(texture, NULL, NULL, &labelWidth, &labelHeight);

            SDL_Rect dstRect = {
                static_cast<int>(textLabel.position.x - (textLabel.isFixed ? 0 : camera.x)),
                static_cast<int>(textLabel.position.y - (textLabel.isFixed ? 0 : camera.y)),
                labelWidth,
                labelHeight
            };
            SDL_RenderCopy(renderer, texture, NULL, &dstRect);
        }
    }
};