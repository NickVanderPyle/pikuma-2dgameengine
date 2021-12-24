#include <fstream>
#include "LevelLoader.h"
#include "../Components/TransformComponent.h"
#include "../Components/SpriteComponent.h"
#include "Game.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/AnimationComponent.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/ProjectileEmitterComponent.h"
#include "../Components/KeyboardControlledComponent.h"
#include "../Components/HealthComponent.h"
#include "../Components/CameraFollowComponent.h"
#include "../Components/ScriptComponent.h"
#include <sol/sol.hpp>

LevelLoader::LevelLoader() {
    Logger::Log("LevelLoader constructor.");
}

LevelLoader::~LevelLoader() {
    Logger::Log("LevelLoader destructor.");
}

void LevelLoader::LoadLevel(sol::state &lua, const std::unique_ptr<Registry> &registry,
                            const std::unique_ptr<AssetStore> &assetStore,
                            SDL_Renderer *renderer, int levelNumber) {
    sol::load_result script = lua.load_file("./assets/scripts/Level" + std::to_string(levelNumber) + ".lua");
    if (!script.valid()) {
        sol::error err = script;
        std::string errorMessage = err.what();
        Logger::Err("Error loading the lua script: " + errorMessage);
        return;
    }

    // Executes the script using the Sol state
    lua.script_file("./assets/scripts/Level" + std::to_string(levelNumber) + ".lua");

    sol::table level = lua["Level"];
    sol::table assets = level["assets"];

    for (int i = 0; i <= assets.size(); i++) {
        sol::table asset = assets[i];
        std::string assetType = asset["type"];
        std::string assetId = asset["id"];
        if (assetType == "texture") {
            assetStore->AddTexture(renderer, assetId, asset["file"]);
            Logger::Log("Texture asset added. id: " + assetId);
        }
        if (assetType == "font") {
            assetStore->AddFont(assetId, asset["file"], asset["font_size"]);
            Logger::Log("Font asset added. id: " + assetId);
        }
    }

    sol::table map = level["tilemap"];
    std::string mapFilePath = map["map_file"];
    std::string mapTextureAssetId = map["texture_asset_id"];
    int mapNumRows = map["num_rows"];
    int mapNumCols = map["num_cols"];
    int tileSize = map["tile_size"];
    double mapScale = map["scale"];
    std::fstream mapFile;
    mapFile.open(mapFilePath);
    for (int y = 0; y < mapNumRows; y++) {
        for (int x = 0; x < mapNumCols; x++) {
            char ch;
            mapFile.get(ch);
            int srcRectY = std::atoi(&ch) * tileSize;
            mapFile.get(ch);
            int srcRectX = std::atoi(&ch) * tileSize;
            mapFile.ignore();

            Entity tile = registry->CreateEntity();
            tile.addComponent<TransformComponent>(glm::vec2(x * (mapScale * tileSize), y * (mapScale * tileSize)),
                                                  glm::vec2(mapScale, mapScale), 0.0);
            tile.addComponent<SpriteComponent>(mapTextureAssetId, tileSize, tileSize, 0, false, srcRectX, srcRectY);
        }
    }
    mapFile.close();
    Game::mapWidth = mapNumCols * tileSize * mapScale;
    Game::mapHeight = mapNumRows * tileSize * mapScale;

    sol::table entities = level["entities"];
    for (int i = 0; i <= entities.size(); i++) {
        sol::table entity = entities[i];

        Entity newEntity = registry->CreateEntity();

        // Tag
        sol::optional<std::string> tag = entity["tag"];
        if (tag != sol::nullopt) {
            newEntity.Tag(entity["tag"]);
        }

        // Group
        sol::optional<std::string> group = entity["group"];
        if (group != sol::nullopt) {
            newEntity.Group(entity["group"]);
        }

        // Components
        sol::optional<sol::table> hasComponents = entity["components"];
        if (hasComponents != sol::nullopt) {
            // Transform
            sol::optional<sol::table> transform = entity["components"]["transform"];
            if (transform != sol::nullopt) {
                newEntity.addComponent<TransformComponent>(
                        glm::vec2(
                                entity["components"]["transform"]["position"]["x"],
                                entity["components"]["transform"]["position"]["y"]
                        ),
                        glm::vec2(
                                entity["components"]["transform"]["scale"]["x"].get_or(1.0),
                                entity["components"]["transform"]["scale"]["y"].get_or(1.0)
                        ),
                        entity["components"]["transform"]["rotation"].get_or(0.0)
                );
            }

            // RigidBody
            sol::optional<sol::table> rigidbody = entity["components"]["rigidbody"];
            if (rigidbody != sol::nullopt) {
                newEntity.addComponent<RigidBodyComponent>(
                        glm::vec2(
                                entity["components"]["rigidbody"]["velocity"]["x"].get_or(0.0),
                                entity["components"]["rigidbody"]["velocity"]["y"].get_or(0.0)
                        )
                );
            }

            // Sprite
            sol::optional<sol::table> sprite = entity["components"]["sprite"];
            if (sprite != sol::nullopt) {
                newEntity.addComponent<SpriteComponent>(
                        entity["components"]["sprite"]["texture_asset_id"],
                        entity["components"]["sprite"]["width"],
                        entity["components"]["sprite"]["height"],
                        entity["components"]["sprite"]["z_index"].get_or(1),
                        entity["components"]["sprite"]["fixed"].get_or(false),
                        entity["components"]["sprite"]["src_rect_x"].get_or(0),
                        entity["components"]["sprite"]["src_rect_y"].get_or(0)
                );
            }

            // Animation
            sol::optional<sol::table> animation = entity["components"]["animation"];
            if (animation != sol::nullopt) {
                newEntity.addComponent<AnimationComponent>(
                        entity["components"]["animation"]["num_frames"].get_or(1),
                        entity["components"]["animation"]["speed_rate"].get_or(1)
                );
            }

            // BoxCollider
            sol::optional<sol::table> collider = entity["components"]["boxcollider"];
            if (collider != sol::nullopt) {
                newEntity.addComponent<BoxColliderComponent>(
                        entity["components"]["boxcollider"]["width"],
                        entity["components"]["boxcollider"]["height"],
                        glm::vec2(
                                entity["components"]["boxcollider"]["offset"]["x"].get_or(0),
                                entity["components"]["boxcollider"]["offset"]["y"].get_or(0)
                        )
                );
            }

            // Health
            sol::optional<sol::table> health = entity["components"]["health"];
            if (health != sol::nullopt) {
                newEntity.addComponent<HealthComponent>(
                        static_cast<int>(entity["components"]["health"]["health_percentage"].get_or(100))
                );
            }

            // ProjectileEmitter
            sol::optional<sol::table> projectileEmitter = entity["components"]["projectile_emitter"];
            if (projectileEmitter != sol::nullopt) {
                newEntity.addComponent<ProjectileEmitterComponent>(
                        glm::vec2(
                                entity["components"]["projectile_emitter"]["projectile_velocity"]["x"],
                                entity["components"]["projectile_emitter"]["projectile_velocity"]["y"]
                        ),
                        static_cast<int>(entity["components"]["projectile_emitter"]["repeat_frequency"].get_or(1)) *
                        1000,
                        static_cast<int>(entity["components"]["projectile_emitter"]["projectile_duration"].get_or(10)) *
                        1000,
                        static_cast<int>(entity["components"]["projectile_emitter"]["hit_percentage_damage"].get_or(
                                10)),
                        entity["components"]["projectile_emitter"]["friendly"].get_or(false)
                );
            }

            // CameraFollow
            sol::optional<sol::table> cameraFollow = entity["components"]["camera_follow"];
            if (cameraFollow != sol::nullopt) {
                newEntity.addComponent<CameraFollowComponent>();
            }

            // KeyboardControlled
            sol::optional<sol::table> keyboardControlled = entity["components"]["keyboard_controller"];
            if (keyboardControlled != sol::nullopt) {
                newEntity.addComponent<KeyboardControlledComponent>(
                        glm::vec2(
                                entity["components"]["keyboard_controller"]["up_velocity"]["x"],
                                entity["components"]["keyboard_controller"]["up_velocity"]["y"]
                        ),
                        glm::vec2(
                                entity["components"]["keyboard_controller"]["right_velocity"]["x"],
                                entity["components"]["keyboard_controller"]["right_velocity"]["y"]
                        ),
                        glm::vec2(
                                entity["components"]["keyboard_controller"]["down_velocity"]["x"],
                                entity["components"]["keyboard_controller"]["down_velocity"]["y"]
                        ),
                        glm::vec2(
                                entity["components"]["keyboard_controller"]["left_velocity"]["x"],
                                entity["components"]["keyboard_controller"]["left_velocity"]["y"]
                        )
                );
            }

            // Script
            sol::optional<sol::table> script = entity["components"]["on_update_script"];
            if (script != sol::nullopt) {
                sol::function func = entity["components"]["on_update_script"][0];
                newEntity.addComponent<ScriptComponent>(func);
            }
        }
    }
}
