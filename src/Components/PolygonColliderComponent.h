#pragma once

#include "../Utils/CookyUtils.h"
#include <box2d/box2d.h>

struct PolygonColliderComponent {
    std::vector<Vec2> vertices;
    int vertexCount;
    Vec2 center;

    PolygonColliderComponent(const std::vector<Vec2>& vertices = std::vector<Vec2>(), int vertexCount = 0, Vec2 center = Vec2::Zero) {
        this->vertices = vertices;
        this->vertexCount = vertexCount;
        this->center = center;
    }
};