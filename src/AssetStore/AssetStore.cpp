//
// Created by Nick VanderPyle on 12/4/21.
//

#include <SDL_image.h>
#include "AssetStore.h"
#include "../Logger/Logger.h"

AssetStore::AssetStore() {
    Logger::Log("AssetStore constructor called");
}

AssetStore::~AssetStore() {
    Logger::Log("AssetStore deconstructor called");
    ClearAssets();
}

void AssetStore::ClearAssets() {
    for (auto texture: textures) {
        SDL_DestroyTexture(texture.second);
    }

    textures.clear();
}

void AssetStore::AddTexture(SDL_Renderer *renderer, const std::string &assetId, const std::string &filePath) {
    SDL_Surface *surface = IMG_Load(filePath.c_str());
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    textures.emplace(assetId, texture);

    Logger::Log("New texture added to the AssetStore with id = " + assetId);
}

SDL_Texture *AssetStore::GetTexture(const std::string &assetId) {
    return textures[assetId];
}
