#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/SpriteComponent.h"
#include <SDL2/SDL.h>

class RenderSystem: public System {
public:
    RenderSystem()
    {
        RequireComponent<TransformComponent>();
        RequireComponent<SpriteComponent>();
    }

    void Update(SDL_Renderer* renderer) {
        for (auto entity : GetSystemEntities()) {
            const TransformComponent transform = entity.GetComponent<TransformComponent>();
            const SpriteComponent sprite = entity.GetComponent<SpriteComponent>();

            // TODO: Draw the PNG texture
        }
    }
};

#endif