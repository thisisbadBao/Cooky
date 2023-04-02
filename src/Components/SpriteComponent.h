#pragma once

#include <string>
#include <SDL2/SDL.h>

#include "../Utils/CookyUtils.h"

enum SpriteFlip
{
    FLIP_NONE,
    FLIP_VERTICAL,
    FLIP_HORIZONTAL
};

struct SpriteComponent {
    std::string assetId;
    int width;
    int height;
    int zIndex;
    SpriteFlip flip;
    bool isFixed;
    Vec2 srcRect;

    SpriteComponent(const std::string& assetId = "", int width = 0, int height = 0, int zIndex = 0,
    bool isFixed = false, Vec2 srcRect = Vec2::Zero) {
        this->assetId = assetId;
        this->width = width;
        this->height = height;
        this->zIndex = zIndex;
        this->flip = FLIP_NONE;
        this->isFixed = isFixed;
        this->srcRect = srcRect;
    }
};