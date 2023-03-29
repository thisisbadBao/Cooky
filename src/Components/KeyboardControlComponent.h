#pragma once

#include "../Utils/CookyUtils.h"

struct KeyboardControlComponent {
    Vec2 upVelocity;
    Vec2 rightVelocity;
    Vec2 downVelocity;
    Vec2 leftVelocity;
    KeyboardControlComponent(Vec2 upVelocity = Vec2::Zero, Vec2 rightVelocity = Vec2::Zero, 
        Vec2 downVelocity = Vec2::Zero, Vec2 leftVelocity = Vec2::Zero) {
        this->upVelocity = upVelocity;
        this->rightVelocity = rightVelocity;
        this->downVelocity = downVelocity;
        this->leftVelocity = leftVelocity;
    }
};