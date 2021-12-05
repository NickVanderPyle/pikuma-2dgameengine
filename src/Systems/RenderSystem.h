//
// Created by Nick VanderPyle on 12/3/21.
//

#ifndef INC_2DGAMEENGINE_RENDERSYSTEM_H
#define INC_2DGAMEENGINE_RENDERSYSTEM_H

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

    void Update(SDL_Renderer *renderer, std::unique_ptr<AssetStore> &assetStore) {
        for (auto entity: GetSystemEntities()) {
            const auto transform = entity.getComponent<TransformComponent>();
            const auto sprite = entity.getComponent<SpriteComponent>();

            SDL_Rect srcRect = sprite.srcRect;

            SDL_Rect dstRect = {
                    static_cast<int>(transform.position.x),
                    static_cast<int>(transform.position.y),
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
                    SDL_FLIP_NONE
            );
        }
    }
};

#endif //INC_2DGAMEENGINE_RENDERSYSTEM_H
