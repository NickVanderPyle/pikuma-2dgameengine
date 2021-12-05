#ifndef INC_2DGAMEENGINE_RENDERCOLLIDERSYSTEM_H
#define INC_2DGAMEENGINE_RENDERCOLLIDERSYSTEM_H

#include <SDL2/SDL.h>
#include "../ECS/EntityAndRegistry.h"
#include "../Components/TransformComponent.h"
#include "../Components/BoxColliderComponent.h"

class RenderColliderSystem : public System {
public:
    RenderColliderSystem() {
        RequireComponent<TransformComponent>();
        RequireComponent<BoxColliderComponent>();
    }

    void Update(SDL_Renderer *renderer) {
        for (auto entity: GetSystemEntities()) {
            const auto transform = entity.getComponent<TransformComponent>();
            const auto collider = entity.getComponent<BoxColliderComponent>();

            SDL_Rect colliderRect = {
                    static_cast<int>(transform.position.x + collider.offset.x),
                    static_cast<int>(transform.position.y + collider.offset.y),
                    static_cast<int>(collider.width),
                    static_cast<int>(collider.height)
            };
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            SDL_RenderDrawRect(renderer, &colliderRect);
        }
    }
};

#endif //INC_2DGAMEENGINE_RENDERCOLLIDERSYSTEM_H
