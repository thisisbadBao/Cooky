#include "Math.h"

const Vec2 Vec2::Zero(0, 0);

Vec2& Vec2::operator=(const Vec2& other) {
    this->x = other.x;
    this->y = other.y;
    return *this;
}

Vec2 Vec2::operator+(const Vec2& other) const {
    return Vec2(x + other.x, y + other.y);
}

Vec2 Vec2::operator-(const Vec2& other) const {
    return Vec2(x - other.x, y - other.y);
}

float Vec2::dot(const Vec2& other) const {
    return x * other.x + y * other.y;
}