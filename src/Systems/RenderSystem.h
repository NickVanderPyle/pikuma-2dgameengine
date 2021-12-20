#pragma once

#include <SDL2/SDL.h>
#include "../ECS/EntityAndRegistry.h"
#include "../Components/TransformComponent.h"
#include "../Components/SpriteComponent.h"

class RenderSystem : public System {
public:
    RenderSystem() {
        RequireComponent<TransformComponent>();
        RequireComponent<SpriteComponent>();
    }

    void Update(SDL_Renderer *renderer, std::unique_ptr<AssetStore> &assetStore, SDL_Rect &camera) {

        struct RenderableEntity {
            TransformComponent transformComponent;
            SpriteComponent spriteComponent;
        };
        std::vector<RenderableEntity> renderableEntities;
        for (auto entity: GetSystemEntities()) {
            RenderableEntity renderableEntity;
            renderableEntity.spriteComponent = entity.getComponent<SpriteComponent>();
            renderableEntity.transformComponent = entity.getComponent<TransformComponent>();
            renderableEntities.emplace_back(renderableEntity);
        }
        std::sort(
                renderableEntities.begin(),
                renderableEntities.end(),
                [](const RenderableEntity &a, const RenderableEntity b) {
                    return a.spriteComponent.zIndex < b.spriteComponent.zIndex;
                });

        for (auto renderableEntity: renderableEntities) {
            const auto transform = renderableEntity.transformComponent;
            const auto sprite = renderableEntity.spriteComponent;

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
