cmake_minimum_required(VERSION 3.20)
project(2dgameengine)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${project_SOURCE_DIR}/cmake")
set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/libs")

add_executable(${PROJECT_NAME} src/main.cpp)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

# SDL2
find_package(SDL2 REQUIRED)
include_directories(${PROJECT_NAME} ${SDL2_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})

find_package(SDL2_image REQUIRED)
include_directories(${PROJECT_NAME} ${SDL2_IMAGE_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} ${SDL2_IMAGE_LIBRARIES})

find_package(SDL2_ttf REQUIRED)
include_directories(${PROJECT_NAME} ${SDLTTF_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} ${SDLTTF_LIBRARY})

find_package(SDL2_mixer REQUIRED)
include_directories(${PROJECT_NAME} ${SDLMIXER_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} ${SDLMIXER_LIBRARY})

find_package(GLM REQUIRED)
message(STATUS "GLM_INCLUDE_DIRS= ${GLM_INCLUDE_DIRS}")
include_directories(${GLM_INCLUDE_DIRS})

set(IMGUI_DIR libs/imgui)
include_directories(libs)
file(GLOB IMGUI_SOURCES ${IMGUI_DIR}/*.cpp ${IMGUI_DIR}/*.h )
add_library(imgui ${IMGUI_SOURCES})
target_link_libraries(${PROJECT_NAME} imgui)