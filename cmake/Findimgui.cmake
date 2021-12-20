include(FetchContent)

message(NOTICE "Fetch Dear ImGui")
FetchContent_Declare(
        imgui
        GIT_REPOSITORY https://github.com/ocornut/imgui.git
        GIT_TAG v1.85
)
FetchContent_MakeAvailable(imgui)

file(GLOB IMGUI_SOURCE
        ${imgui_SOURCE_DIR}/*.cpp
        ${imgui_SOURCE_DIR}/*.h
        ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl.h
        ${imgui_SOURCE_DIR}/backends/imgui_impl_sdlrenderer.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_sdlrenderer.h
)

add_library(
        imgui STATIC
        ${IMGUI_SOURCE}
)

target_include_directories(
        imgui PUBLIC
        ${imgui_SOURCE_DIR}
        ${imgui_SOURCE_DIR}/backends
)
