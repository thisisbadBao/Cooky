#pragma once

#include <SDL2/SDL.h>
#include <box2d/box2d.h>
#include <math.h>

#include "../ECS/ECS.h"
#include "../Utils/CookyUtils.h"
#include "../Components/TransformComponent.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/PolygonColliderComponent.h"
#include "../Components/CircleColliderComponnet.h"
#include "../Components/ColliderComponent.h"

class RenderColliderSystem: public System {
public:
    RenderColliderSystem() {
        RequireComponent<TransformComponent>();
        RequireComponent<ColliderComponent>();
    }

    void Update(std::unique_ptr<DebugDraw>& debugDraw, SDL_Rect& camera) {
        for (auto entity : GetSystemEntities()) {
            // auto transform = entity.GetComponent<TransformComponent>();
            const auto collider = entity.GetComponent<ColliderComponent>();
            Color drawColor = {67, 160, 71, 255};

            if (collider.shape == CShape::POLYGON) {
                const PolygonColliderComponent poly = entity.GetComponent<PolygonColliderComponent>();
                debugDraw->DrawPolygon(poly.vertices, poly.vertexCount, drawColor);
            }

            if (collider.shape == CShape::CIRCLE) {
                const CircleColliderComponnet circle = entity.GetComponent<CircleColliderComponnet>();
                debugDraw->DrawCircle(circle.center, circle.radius, drawColor);
            }
        }
    }
};