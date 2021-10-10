cmake_minimum_required(VERSION 3.20)
project(2dgameengine)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${project_SOURCE_DIR}/cmake")
set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/libs")

include_directories(${LIB_DIR})
add_executable(${PROJECT_NAME} ${SRC_DIR}/main.cpp src/Game.cpp src/Game.h)

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
