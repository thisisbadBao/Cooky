#pragma once

#include <SDL2/SDL.h>
#include <box2d/box2d.h>
#include <math.h>

#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/PolygonColliderComponent.h"
#include "../Components/ColliderComponent.h"

class RenderColliderSystem: public System {
public:
    RenderColliderSystem() {
        RequireComponent<TransformComponent>();
        RequireComponent<ColliderComponent>();
    }

    void Update(SDL_Renderer* renderer, SDL_Rect& camera) {
        for (auto entity : GetSystemEntities()) {
            // auto transform = entity.GetComponent<TransformComponent>();
            const auto collider = entity.GetComponent<ColliderComponent>();
            Color drawColor = {255, 0, 0, 255};

            if (collider.shape == CShape::POLYGON) {
                const PolygonColliderComponent poly = entity.GetComponent<PolygonColliderComponent>();
                for (int i = 0; i < poly.count - 1; i++) {
                    SDL_SetRenderDrawColor(renderer, drawColor.r, drawColor.g, drawColor.b, drawColor.a);
                    for (int j = i + 1; j < poly.count; j++) {
                        SDL_RenderDrawLine(
                            renderer,
                            poly.points[i].x * MET2PIX,
                            poly.points[i].y * MET2PIX,
                            poly.points[j].x * MET2PIX,
                            poly.points[j].y * MET2PIX
                        );
                    }
                }
            }
        }
    }
};