include(FetchContent)

message(NOTICE "Fetch Sol")
FetchContent_Declare(
        sol
        GIT_REPOSITORY https://github.com/ThePhD/sol2.git
        GIT_TAG v3.2.3
        SOURCE_SUBDIR single/
)
FetchContent_MakeAvailable(sol)

file(GLOB SOL_SOURCE
        ${sol_SOURCE_DIR}/single/include/sol/*.hpp
)

add_library(
        sol STATIC
        ${SOL_SOURCE}
)

set_target_properties(sol PROPERTIES LINKER_LANGUAGE CXX)
include_directories(
        sol PUBLIC
        ${sol_SOURCE_DIR}/single/include/
)
