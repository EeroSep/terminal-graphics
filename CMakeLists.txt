cmake_minimum_required(VERSION 3.16)
project(terminal_graphics LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ---- Find system Notcurses ----
find_package(PkgConfig REQUIRED)
pkg_check_modules(NOTCURSES REQUIRED notcurses)

# ---- Add executable ----
add_executable(terminal_graphics
    src/main.cpp
    src/math/Mat4.cpp
    src/math/Vec3.cpp
    src/math/Vec4.cpp
    src/math/Vec2.cpp
    src/math/graphic_matrices.h
    src/engine/Camera.cpp
    src/engine/Framebuffer.cpp
    src/engine/InputHandler.cpp
    src/engine/Light.h
    src/engine/Material.cpp
    src/engine/Object.cpp
    src/engine/Rasterizer.cpp
    src/engine/Renderer.cpp
)

# ---- Include directories ----
target_include_directories(terminal_graphics PRIVATE
    ${NOTCURSES_INCLUDE_DIRS}
    src/math
    src/engine
    src/utility
)

# ---- Link libraries ----
target_link_libraries(terminal_graphics PRIVATE ${NOTCURSES_LIBRARIES})

# ---- Compiler flags from pkg-config ----
target_compile_options(terminal_graphics PRIVATE ${NOTCURSES_CFLAGS_OTHER})

