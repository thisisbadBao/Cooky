#pragma once

#include "../Utils/CookyUtils.h"
#include <box2d/box2d.h>

struct PolygonColliderComponent {
    std::vector<Vec2> points;
    int count;
    Vec2 center;

    PolygonColliderComponent(const std::vector<Vec2>& points = std::vector<Vec2>(), int count = 0, Vec2 center = Vec2::Zero) {
        this->points = points;
        this->count = count;
        this->center = center;
    }
};