#pragma once

#include "../Utils/CookyUtils.h"
struct TransformComponent {
    Vec2 position;
    Vec2 scale;
    double rotation;

    TransformComponent(Vec2 position = Vec2(0,0), Vec2 scale = Vec2(1,1), double rotation = 0.0) {
        this->position = position;
        this->scale = scale;
        this->rotation = rotation;
    }
};