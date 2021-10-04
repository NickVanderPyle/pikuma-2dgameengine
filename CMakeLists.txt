cmake_minimum_required(VERSION 3.20)
project(2dgameengine)

set(CMAKE_CXX_STANDARD 17)

set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/libs")

add_executable(2dgameengine src/main.cpp)

# SDL2
find_package(SDL2 REQUIRED)
include_directories(2dgameengine ${SDL2_INCLUDE_DIRS})
target_link_libraries(2dgameengine ${SDL2_LIBRARIES})
