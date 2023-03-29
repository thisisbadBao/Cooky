#pragma once

#include <string>
#include "../Utils/CookyUtils.h"
#include <SDL2/SDL.h>

struct TextLabelComponent {
    Vec2 position;
    std::string text;
    std::string assetId;
    SDL_Color color;
    bool isFixed;

    TextLabelComponent(Vec2 position = Vec2::Zero, std::string text = "",
    std::string assetId = "", const SDL_Color& color = {0, 0, 0}, bool isFixed = true) {
        this->position = position;
        this->text = text;
        this->assetId = assetId;
        this->color = color;
        this->isFixed = isFixed;
    }
};