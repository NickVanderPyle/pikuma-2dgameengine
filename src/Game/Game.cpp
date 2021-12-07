#include "Game.h"
#include "../Logger/Logger.h"
#include "../Components/TransformComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Systems/MovementSystem.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <glm/glm.hpp>
#include <fstream>
#include "../ECS/EntityAndRegistry.h"
#include "../Systems/RenderSystem.h"
#include "../AssetStore/AssetStore.h"
#include "../Components/AnimationComponent.h"
#include "../Systems/AnimationSystem.h"
#include "../Components/BoxColliderComponent.h"
#include "../Systems/CollisionSystem.h"
#include "../Systems/RenderColliderSystem.h"
#include "../Systems/DamageSystem.h"
#include "../Systems/KeyboardControlSystem.h"
#include "../Components/KeyboardControlledComponent.h"
#include "../Components/CameraFollowComponent.h"
#include "../Systems/CameraMovementSystem.h"
#include "../Components/ProjectileEmitterComponent.h"
#include "../Systems/ProjectileEmitSystem.h"
#include "../Components/HealthComponent.h"
#include "../Systems/ProjectileLifecycleSystem.h"

int Game::windowWidth;
int Game::windowHeight;
int Game::mapWidth;
int Game::mapHeight;

Game::Game() {
    Logger::Log("Game constructor");
    isRunning = false;
    isDebugging = false;
    registry = std::make_unique<Registry>();
    assetStore = std::make_unique<AssetStore>();
    eventBus = std::make_unique<EventBus>();
}

Game::~Game() {
    Logger::Log("Game destructor");
}

void Game::Initialize() {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        Logger::Err("Error SDL_Init");
        return;
    }

    SDL_DisplayMode displayMode;
    SDL_GetCurrentDisplayMode(0, &displayMode);
    windowWidth = displayMode.w;
    windowHeight = displayMode.h;

    window = SDL_CreateWindow(
            NULL,
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            windowWidth,
            windowHeight,
            SDL_WINDOW_BORDERLESS);
    if (!window) {
        Logger::Err("Error SDL_CreateWindow");
        return;
    }

    renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer) {
        Logger::Err("Error SDL_CreateRenderer");
        return;
    }

    camera.x = 0;
    camera.y = 0;
    camera.w = windowWidth;
    camera.h = windowHeight;

    SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);

    isRunning = true;
}

void Game::Run() {
    Setup();

    while (isRunning) {
        ProcessInput();
        Update();
        Render();
    }
}

void Game::ProcessInput() {
    SDL_Event sdlEvent;
    while (SDL_PollEvent(&sdlEvent)) {
        switch (sdlEvent.type) {
            case SDL_QUIT:
                isRunning = false;
                break;
            case SDL_KEYDOWN:
                if (sdlEvent.key.keysym.sym == SDLK_ESCAPE) {
                    isRunning = false;
                }
                if (sdlEvent.key.keysym.sym == SDLK_d) {
                    isDebugging = !isDebugging;
                }
                eventBus->EmitEvent<KeyPressedEvent>(sdlEvent.key.keysym.sym);
                break;
        }
    }
}

void Game::LoadLevel(int level) {
    registry->AddSystem<MovementSystem>();
    registry->AddSystem<RenderSystem>();
    registry->AddSystem<AnimationSystem>();
    registry->AddSystem<CollisionSystem>();
    registry->AddSystem<RenderColliderSystem>();
    registry->AddSystem<DamageSystem>();
    registry->AddSystem<KeyboardControlSystem>();
    registry->AddSystem<CameraMovementSystem>();
    registry->AddSystem<ProjectileEmitSystem>();
    registry->AddSystem<ProjectileLifecycleSystem>();

    assetStore->AddTexture(renderer, "radar-image", "./assets/images/radar.png");
    assetStore->AddTexture(renderer, "chopper-image", "./assets/images/chopper-spritesheet.png");
    assetStore->AddTexture(renderer, "tank-image", "./assets/images/tank-panther-right.png");
    assetStore->AddTexture(renderer, "truck-image", "./assets/images/truck-ford-right.png");
    assetStore->AddTexture(renderer, "tilemap-image", "./assets/tilemaps/jungle.png");
    assetStore->AddTexture(renderer, "bullet-image", "./assets/images/bullet.png");

    int tileSize = 32;
    double tileScale = 2.0;
    int mapNumCols = 25;
    int mapNumRows = 20;
    std::fstream mapFile;
    mapFile.open("./assets/tilemaps/jungle.map");
    for (int y = 0; y < mapNumRows; y++) {
        for (int x = 0; x < mapNumCols; x++) {
            char ch;
            mapFile.get(ch);
            int srcRectY = std::atoi(&ch) * tileSize;
            mapFile.get(ch);
            int srcRectX = std::atoi(&ch) * tileSize;
            mapFile.ignore();

            Entity tile = registry->CreateEntity();
            tile.addComponent<TransformComponent>(
                    glm::vec2(x * (tileScale * tileSize), y * (tileScale * tileSize)),
                    glm::vec2(tileScale, tileScale),
                    0.0);
            tile.addComponent<SpriteComponent>("tilemap-image", tileSize, tileSize, 0, false, srcRectX, srcRectY);
        }
    }
    mapFile.close();
    mapWidth = mapNumCols * tileSize * tileScale;
    mapHeight = mapNumRows * tileSize * tileScale;

    Entity chopper = registry->CreateEntity();
    chopper.addComponent<TransformComponent>(glm::vec2(10.0, 100.0), glm::vec2(1.0, 1.0), 0.0);
    chopper.addComponent<RigidBodyComponent>(glm::vec2(0.0, 0.0));
    chopper.addComponent<SpriteComponent>("chopper-image", 32, 32, 1);
    chopper.addComponent<AnimationComponent>(2, 15, true);
    chopper.addComponent<ProjectileEmitterComponent>(glm::vec2(150.0, 150.0), 0, 3000, 0, true);
    chopper.addComponent<KeyboardControlledComponent>(glm::vec2(0, -80), glm::vec2(80, 0), glm::vec2(0, 80),
                                                      glm::vec2(-80, 0));
    chopper.addComponent<CameraFollowComponent>();
    chopper.addComponent<HealthComponent>(100);

    Entity radar = registry->CreateEntity();
    radar.addComponent<TransformComponent>(glm::vec2(windowWidth - 74, 10), glm::vec2(1.0, 1.0), 0.0);
    radar.addComponent<RigidBodyComponent>(glm::vec2(0.0, 0.0));
    radar.addComponent<SpriteComponent>("radar-image", 64, 64, 1, true);
    radar.addComponent<AnimationComponent>(8, 5, true);

    Entity tank = registry->CreateEntity();
    tank.addComponent<TransformComponent>(glm::vec2(500.0, 10.0), glm::vec2(1.0, 1.0), 0.0);
    tank.addComponent<RigidBodyComponent>(glm::vec2(0.0, 0.0));
    tank.addComponent<SpriteComponent>("tank-image", 32, 32, 1);
    tank.addComponent<BoxColliderComponent>(32, 32, glm::vec2(0, 0));
    tank.addComponent<ProjectileEmitterComponent>(glm::vec2(100.0, 0.0), 5000, 3000, 0, false);
    tank.addComponent<HealthComponent>(100);

    Entity truck = registry->CreateEntity();
    truck.addComponent<TransformComponent>(glm::vec2(10.0, 10.0), glm::vec2(1.0, 1.0), 0.0);
    truck.addComponent<RigidBodyComponent>(glm::vec2(0.0, 0.0));
    truck.addComponent<SpriteComponent>("truck-image", 32, 32, 2);
    truck.addComponent<BoxColliderComponent>(32, 32, glm::vec2(0, 0));
    truck.addComponent<ProjectileEmitterComponent>(glm::vec2(0.0, 100.0), 2000, 5000, 0, false);
    truck.addComponent<HealthComponent>(100);
}

void Game::Setup() {
    LoadLevel(1);
}

void Game::Update() {
    int timeToWait = MILLISECS_PER_FRAME - (SDL_GetTicks() - millisecsPreviousFrame);
    if (timeToWait > 0 && timeToWait <= MILLISECS_PER_FRAME) {
        SDL_Delay(timeToWait);
    }
    double deltaTime = (SDL_GetTicks() - millisecsPreviousFrame) / 1000.0f;
    millisecsPreviousFrame = SDL_GetTicks();

    eventBus->Reset();
    registry->GetSystem<DamageSystem>().SubscribeToEvents(eventBus);
    registry->GetSystem<KeyboardControlSystem>().SubscribeToEvents(eventBus);
    registry->GetSystem<ProjectileEmitSystem>().SubscribeToEvents(eventBus);

    registry->Update();

    registry->GetSystem<MovementSystem>().Update(deltaTime);
    registry->GetSystem<AnimationSystem>().Update();
    registry->GetSystem<CollisionSystem>().Update(eventBus);
    registry->GetSystem<ProjectileEmitSystem>().Update(registry);
    registry->GetSystem<CameraMovementSystem>().Update(camera);
    registry->GetSystem<ProjectileLifecycleSystem>().Update();
}

void Game::Render() {
    SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);
    SDL_RenderClear(renderer);

    registry->GetSystem<RenderSystem>().Update(renderer, assetStore, camera);
    if (isDebugging) {
        registry->GetSystem<RenderColliderSystem>().Update(renderer, camera);
    }

    SDL_RenderPresent(renderer);
}

void Game::Destroy() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}