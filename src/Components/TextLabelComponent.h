#pragma once

#include <string>
#include <SDL2/SDL.h>

#include "../Utils/CookyUtils.h"

struct TextLabelComponent {
    Vec2 position;
    std::string text;
    std::string assetId;
    Vec3 color;
    bool isFixed;

    TextLabelComponent(Vec2 position = Vec2::Zero, std::string text = "",
    std::string assetId = "", Vec3 color = Vec3::Zero, bool isFixed = true) {
        this->position = position;
        this->text = text;
        this->assetId = assetId;
        this->color = color;
        this->isFixed = isFixed;
    }
};