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

    for (auto font: fonts) {
        TTF_CloseFont(font.second);
    }
    fonts.clear();
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

void AssetStore::AddFont(const std::string &assetId, const std::string &filePath, int fontSize) {
    fonts.emplace(assetId, TTF_OpenFont(filePath.c_str(), fontSize));
}

TTF_Font *AssetStore::GetFont(const std::string &assetId) {
    return fonts[assetId];
}
