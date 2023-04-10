#pragma once

#include "../Utils/CookyUtils.h"

enum CShape {
    CIRCLE = 0,
    EDGE = 1,
    POLYGON = 2,
    CHAIN = 3,
    NO_SHAPE,
};

struct ColliderComponent {
    CShape shape;

    ColliderComponent(CShape shape = NO_SHAPE) {
        this->shape = shape;
    }
};