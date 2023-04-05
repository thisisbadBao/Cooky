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

const Vec3 Vec3::Zero(0, 0, 0);

Vec3& Vec3::operator=(const Vec3& other) {
    this->x = other.x;
    this->y = other.y;
    this->z = other.z;
    return *this;
}

Vec3 Vec3::operator+(const Vec3& other) const {
    return Vec3(x + other.x, y + other.y, z + other.z);
}

Vec3 Vec3::operator-(const Vec3& other) const {
    return Vec3(x - other.x, y - other.y, z - other.z);
}

Color::Color() {
    this->r = 255;
    this->g = 255;
    this->b = 255;
    this->a = 255;
}

Color::Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    this->r = r;
    this->g = g;
    this->b = b;
    this->a = a;
}

Color Color::White;