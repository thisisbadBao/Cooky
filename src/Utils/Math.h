#pragma once

#include <stdint.h>

struct Vec2 {
    float x;
    float y;

    Vec2() : x(0), y(0) {}

    Vec2(float x, float y) : x(x), y(y) {}

    Vec2 &operator=(const Vec2& other);

    Vec2 operator+(const Vec2& other) const;

    Vec2 operator-(const Vec2& other) const;

    Vec2& operator-=(const Vec2& other);

    Vec2& operator+=(const Vec2& other);

    float dot(const Vec2& other) const;

    float distance(const Vec2& other) const;

    void Set(float x, float y);

    void Rotate(Vec2 center, float angle);

    static const Vec2 Zero;

};

struct Vec3 {
    float x;
    float y;
    float z;

    Vec3() : x(0), y(0), z(0) {}

    Vec3(float x, float y, float z) : x(x), y(y), z(z) {}

    Vec3 &operator=(const Vec3& other);

    Vec3 operator+(const Vec3& other) const;

    Vec3 operator-(const Vec3& other) const;

    static const Vec3 Zero;
};

struct Vec4 {
    float x;
    float y;
    float z;
    float w;
};

struct Color {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;

    Color();
    Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
    Color& operator=(const Color& color);
    static Color White;
};
