#include "Math.h"
#include <math.h>

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

Vec2& Vec2::operator-=(const Vec2& other) {
    x -= other.x;
    y -= other.y;
    return *this;
}

Vec2& Vec2::operator+=(const Vec2& other) {
    x += other.x;
    y += other.y;
    return *this;
}

float Vec2::dot(const Vec2& other) const {
    return x * other.x + y * other.y;
}

float Vec2::distance(const Vec2 &other) const {
    float dx = x - other.x;
    float dy = y - other.y;
    return sqrt(dx * dx + dy * dy);
}

void Vec2::Set(float _x, float _y) {
    this->x = _x;
    this->y = _y;
}

void Vec2::Rotate(Vec2 center, float angle) {
    float s = sin(angle);
    float c = cos(angle);
    float xtemp = x - center.x;
    float ytemp = y - center.y;
    x = xtemp * c - ytemp * s + center.x;
    y = xtemp * s + ytemp * c + center.y;
}

std::string Vec2::ToString() {
    return "(" + std::to_string(x) + ", " + std::to_string(y) + ")";
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

Color& Color::operator=(const Color& color) {
    this->r = color.r;
    this->g = color.g;
    this->b = color.b;
    this->a = color.a;
    return *this;
}

Color Color::White;