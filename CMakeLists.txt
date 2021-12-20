cmake_minimum_required(VERSION 3.20)
include(FetchContent)
project(2dgameengine)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${project_SOURCE_DIR}/cmake")
set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/libs")

file(GLOB_RECURSE APP_HEADERS CONFIGURE_DEPENDS "${SRC_DIR}/*.h")
file(GLOB_RECURSE APP_CPP_SOURCES CONFIGURE_DEPENDS "${SRC_DIR}/*.hpp")
file(GLOB_RECURSE APP_SOURCES CONFIGURE_DEPENDS "${SRC_DIR}/*.cpp")

add_executable(${PROJECT_NAME} ${APP_HEADERS} ${APP_CPP_SOURCES} ${APP_SOURCES} src/Components/TransformComponent.h src/Systems/MovementSystem.h src/ECS/Signature.h src/ECS/IComponent.h src/ECS/Component.h src/ECS/System.cpp src/ECS/System.h src/ECS/IPool.h src/ECS/Pool.h src/ECS/Component.cpp src/ECS/EntityAndRegistry.cpp src/ECS/EntityAndRegistry.h src/Components/RigidBodyComponent.h src/Components/SpriteComponent.h src/Systems/RenderSystem.h src/AssetStore/AssetStore.cpp src/AssetStore/AssetStore.h src/Components/AnimationComponent.h src/Systems/AnimationSystem.h src/Components/BoxColliderComponent.h src/Systems/CollisionSystem.h src/Systems/RenderColliderSystem.h src/Events/CollisionEvent.h src/EventBus/EventBus.h src/EventBus/Event.h src/EventBus/IEventCallback.h src/EventBus/EventCallback.h src/Systems/DamageSystem.h src/Events/KeyPressedEvent.h src/Systems/KeyboardControlSystem.h src/Components/KeyboardControlledComponent.h src/Components/CameraFollowComponent.h src/Systems/CameraMovementSystem.h src/Components/ProjectileEmitterComponent.h src/Systems/ProjectileEmitSystem.h src/Components/HealthComponent.h src/Components/ProjectileComponent.h src/Systems/ProjectileLifecycleSystem.h src/Components/TextLabelComponent.h src/Systems/RenderTextSystem.h src/Systems/RenderHealthBarSystem.h src/Systems/RenderGuiSystem.h)
include_directories(${LIB_DIR})

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

# brew install sdl2
find_package(SDL2 REQUIRED)
include_directories(${PROJECT_NAME} ${SDL2_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})

# brew install SDL2_image
find_package(SDL2_image REQUIRED)
include_directories(${PROJECT_NAME} ${SDL2_IMAGE_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} ${SDL2_IMAGE_LIBRARIES})

# brew install SDL2_ttf
find_package(SDL2_ttf REQUIRED)
include_directories(${PROJECT_NAME} ${SDLTTF_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} ${SDLTTF_LIBRARY})

# brew install SDL2_mixer
find_package(SDL2_mixer REQUIRED)
include_directories(${PROJECT_NAME} ${SDLMIXER_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} ${SDLMIXER_LIBRARY})

# brew install glm
find_package(GLM REQUIRED)
include_directories(${GLM_INCLUDE_DIRS})

# brew install lua
find_package(Lua REQUIRED)
target_link_libraries(${PROJECT_NAME} ${LUA_LIBRARIES})
target_include_directories(${PROJECT_NAME} PRIVATE ${LUA_INCLUDE_DIR})

find_package(imgui REQUIRED)
target_link_libraries(${PROJECT_NAME} imgui)

add_custom_target(copy_assets
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets/
        ${CMAKE_BINARY_DIR}/assets/
        COMMENT "Copying assets..."
        VERBATIM)
add_dependencies(${PROJECT_NAME} copy_assets)