#pragma once

#include "../Utils/CookyUtils.h"

struct KeyboardControlComponent {
    Vec2 upvel;
    Vec2 rightvel;
    Vec2 downvel;
    Vec2 leftvel;
    KeyboardControlComponent(Vec2 upvel = Vec2::Zero, Vec2 rightvel = Vec2::Zero, 
        Vec2 downvel = Vec2::Zero, Vec2 leftvel = Vec2::Zero) {
        this->upvel = upvel;
        this->rightvel = rightvel;
        this->downvel = downvel;
        this->leftvel = leftvel;
    }
};