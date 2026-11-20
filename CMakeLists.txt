cmake_minimum_required(VERSION 3.12)
project(cardGame C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)

# Every .cpp inside src/ is picked up automatically.
# CONFIGURE_DEPENDS makes CMake notice when you add a new file.
file(GLOB SOURCES CONFIGURE_DEPENDS src/*.cpp)

# glad.c is plain C, so we add it by hand.
add_executable(cardGame ${SOURCES} src/glad.c)

# include/ holds glad, KHR and stb_image.h. src/ lets us write #include "Shader.h".
target_include_directories(cardGame PRIVATE include src)

# The code finds shaders/ and textures/ through this path, so the game
# runs from any folder (terminal, VS Code F5, anywhere).
target_compile_definitions(cardGame PRIVATE ASSET_DIR="${CMAKE_SOURCE_DIR}/")

target_link_libraries(cardGame PRIVATE glfw OpenGL::GL dl)
