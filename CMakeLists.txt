cmake_minimum_required(VERSION 3.16)
project(terminal_graphics LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# ---- GLM (header-only) ----
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 0.9.9.8
)
FetchContent_MakeAvailable(glm)

# ---- Find system Notcurses ----
find_package(PkgConfig REQUIRED)
pkg_check_modules(NOTCURSES REQUIRED notcurses)

# ---- Add executable ----
add_executable(terminal_graphics src/main.cpp)

# ---- Include directories ----
target_include_directories(terminal_graphics PRIVATE
    ${glm_SOURCE_DIR}
    ${NOTCURSES_INCLUDE_DIRS}
)

# ---- Link libraries ----
target_link_libraries(terminal_graphics PRIVATE glm::glm ${NOTCURSES_LIBRARIES})

# ---- Compiler flags from pkg-config ----
target_compile_options(terminal_graphics PRIVATE ${NOTCURSES_CFLAGS_OTHER})