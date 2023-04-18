#pragma once

#include <SDL2/SDL.h>
#include <vector>

#include "CookyUtils.h"

class DebugDraw {
public:
    void DrawPolygon(const std::vector<Vec2> vertices, int vertexCount, const Color& color);
    void DrawSolidPolygon(const Vec2* vertices, int vertexCount, const Color& color);
    void DrawCircle(const Vec2& center, float radius, const Color& color);
    void DrawSolidCircle(const Vec2& center, float radius, const Vec2& axis, const Color& color);
    void DrawSegment(const Vec2& p1, const Vec2& p2, const Color& color);
    void DrawTransform(const Vec2& xf);
    void DrawPoint(const Vec2& p, float size, const Color& color);

    void SetRenderer(SDL_Renderer *renderer);
private:
    SDL_Renderer* renderer;
};