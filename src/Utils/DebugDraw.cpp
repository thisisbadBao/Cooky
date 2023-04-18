#include "DebugDraw.h"

void DebugDraw::DrawPolygon(const std::vector<Vec2> vertices, int vertexCount, const Color& color) {
    for (int i = 0; i < vertexCount - 1; i++) {
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
        for (int j = i + 1; j < vertexCount; j++) {
            SDL_RenderDrawLine(
                renderer,
                vertices[i].x * MET2PIX,
                vertices[i].y * MET2PIX,
                vertices[j].x * MET2PIX,
                vertices[j].y * MET2PIX
            );
        }
    }
}

void DebugDraw::DrawCircle(const Vec2& center, float radius, const Color& color) {
    const int segments = 16;
    const float increment = 2.0f * M_PI / segments;
    float sinIn = sin(increment);
    float cosIn = cos(increment);
    Vec2 cur(0, radius);
    cur += center;
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    for (int i = 0; i < segments; i++) {
        Vec2 next;
        float xtemp = cur.x - center.x;
        float ytemp = cur.y - center.y;
        next.x = xtemp * cosIn - ytemp * sinIn + center.x;
        next.y = xtemp * sinIn + ytemp * cosIn + center.y;
        SDL_RenderDrawLine(
            renderer,
            cur.x * MET2PIX,
            cur.y * MET2PIX,
            next.x * MET2PIX,
            next.y * MET2PIX
        );
        cur = next;
    }
}

void DebugDraw::SetRenderer(SDL_Renderer *renderer) {
    this->renderer = renderer;
}

// To be implemented
void DebugDraw::DrawSolidPolygon(const Vec2* vertices, int vertexCount, const Color& color) {}
void DebugDraw::DrawSolidCircle(const Vec2& center, float radius, const Vec2& axis, const Color& color) {}
void DebugDraw::DrawSegment(const Vec2& p1, const Vec2& p2, const Color& color) {}
void DebugDraw::DrawTransform(const Vec2& xf) {}
void DebugDraw::DrawPoint(const Vec2& p, float size, const Color& color) {}