#pragma once

#include <SDL2/SDL.h>

struct SpriteComponent {
    std::string assetId;
    int width;
    int height;
    int zIndex;
    SDL_RendererFlip flip;
    bool isFixed;
    SDL_Rect srcRect;

    SpriteComponent(std::string assetId = "", int width = 0, int height = 0, int zIndex = 0, bool isFixed = false,
                    int srcRectX = 0,
                    int srcRectY = 0) {
        this->assetId = assetId;
        this->width = width;
        this->height = height;
        this->zIndex = zIndex;
        this->flip = SDL_FLIP_NONE;
        this->isFixed = isFixed;
        this->srcRect = {srcRectX, srcRectY, width, height};
    }
};
