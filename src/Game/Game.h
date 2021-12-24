#pragma once

#include <memory>
#include <SDL2/SDL.h>
#include "../ECS/EntityAndRegistry.h"
#include "../AssetStore/AssetStore.h"
#include "../EventBus/EventBus.h"
#include <sol/sol.hpp>

const int FPS = 30;

const int MILLISECS_PER_FRAME = 1000 / FPS;

class Game {
private:
    bool isRunning;
    bool isDebugging;
    int millisecsPreviousFrame = 0;
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Rect camera;

    sol::state lua;

    std::unique_ptr<Registry> registry;
    std::unique_ptr<AssetStore> assetStore;
    std::unique_ptr<EventBus> eventBus;

public:
    Game();

    ~Game();

    void Initialize();

    void Run();

    void ProcessInput();

    void Setup();

    void Update();

    void Render();

    void Destroy();

    static int windowWidth;
    static int windowHeight;
    static int mapWidth;
    static int mapHeight;
};
