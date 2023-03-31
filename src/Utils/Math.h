#pragma once

struct Vec2 {
    float x;
    float y;

    Vec2() : x(0), y(0) {}

    Vec2(float x, float y) : x(x), y(y) {}

    Vec2 &operator=(const Vec2 &other);

    Vec2 operator+(const Vec2 &other) const;

    Vec2 operator-(const Vec2 &other) const;

    float dot(const Vec2 &other) const;

    static const Vec2 Zero;

};

struct Vec3 {
    float x;
    float y;
    float z;

    Vec3() : x(0), y(0), z(0) {}

    Vec3(float x, float y, float z) : x(x), y(y), z(z) {}

    Vec3 &operator=(const Vec3 &other);

    Vec3 operator+(const Vec3 &other) const;

    Vec3 operator-(const Vec3 &other) const;

    static const Vec3 Zero;

};
