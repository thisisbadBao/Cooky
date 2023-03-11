#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/SpriteComponent.h"
#include "../AssetStore/AssetStore.h"
#include <SDL2/SDL.h>

struct RenderableEntity {
    TransformComponent transformComponent;
    SpriteComponent spriteComponent;
};

class RenderSystem: public System {
public:
    RenderSystem()
    {
        RequireComponent<TransformComponent>();
        RequireComponent<SpriteComponent>();
    }

    void Update(SDL_Renderer* renderer, std::unique_ptr<AssetStore>& assetStore, SDL_Rect camera) {
        // Sort all the entities by z-index
        std::vector<RenderableEntity> renderableEntities;
        for (auto entity : GetSystemEntities())
        {
            RenderableEntity renderableEntity;
            renderableEntity.transformComponent = entity.GetComponent<TransformComponent>();
            renderableEntity.spriteComponent = entity.GetComponent<SpriteComponent>();

            // Bypass entity if it's outside the camera
            bool isEntityOutsideCamera = (
                renderableEntity.transformComponent.position.x + renderableEntity.spriteComponent.width * renderableEntity.transformComponent.scale.x < camera.x ||
                renderableEntity.transformComponent.position.x > camera.x + camera.w ||
                renderableEntity.transformComponent.position.y + renderableEntity.spriteComponent.height * renderableEntity.transformComponent.scale.y < camera.y ||
                renderableEntity.transformComponent.position.y > camera.y + camera.h
            );
            if (isEntityOutsideCamera && !renderableEntity.spriteComponent.isFixed) continue;

            renderableEntities.emplace_back(renderableEntity);
        }
        std::sort(renderableEntities.begin(), renderableEntities.end(),
            [](const RenderableEntity &a, const RenderableEntity &b)
            {
                return a.spriteComponent.zIndex < b.spriteComponent.zIndex;
            });

        for (auto entity : renderableEntities) {
            const TransformComponent transform = entity.transformComponent;
            const SpriteComponent sprite = entity.spriteComponent;

            SDL_Rect srcRect = sprite.srcRect;
            SDL_Rect dstRect = {
                static_cast<int>(transform.position.x - (sprite.isFixed ? 0 : camera.x)),
                static_cast<int>(transform.position.y - (sprite.isFixed ? 0 : camera.y)),
                static_cast<int>(sprite.width * transform.scale.x),
                static_cast<int>(sprite.height * transform.scale.y)
            };

            SDL_RenderCopyEx(
                renderer,
                assetStore->GetTexture(sprite.assetId),
                &srcRect,
                &dstRect,
                transform.rotation,
                NULL,
                sprite.flip
            );
        }
    }
};

#endif