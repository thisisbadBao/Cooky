#pragma once

#include "../Utils/CookyUtils.h"

struct BoxColliderComponent {
    int width;
    int height;
    Vec2 offset;
    bool isColliding;

    BoxColliderComponent(int width = 0, int height = 0, Vec2 offset = Vec2::Zero) {
        this->width = width;
        this->height = height;
        this->offset = offset;
        this->isColliding = false;
    }
};