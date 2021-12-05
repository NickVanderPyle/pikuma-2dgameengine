//
// Created by Nick VanderPyle on 12/4/21.
//

#ifndef INC_2DGAMEENGINE_ASSETSTORE_H
#define INC_2DGAMEENGINE_ASSETSTORE_H


#include <map>
#include <SDL_render.h>

class AssetStore {
private:
    std::map<std::string, SDL_Texture *> textures;
    // TODO: create a map for fonts
    // TODO: create a map for audio

public:
    AssetStore();

    ~AssetStore();

    void ClearAssets();

    void AddTexture(SDL_Renderer *renderer, const std::string &assetId, const std::string &filePath);

    SDL_Texture *GetTexture(const std::string &assetId);


};


#endif //INC_2DGAMEENGINE_ASSETSTORE_H
