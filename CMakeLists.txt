cmake_minimum_required(VERSION 3.12)

project(cardGame C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# Use installed GLFW if found, otherwise download and build it automatically
find_package(glfw3 QUIET)
if(NOT glfw3_FOUND)
    set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    FetchContent_Declare(glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG 3.4
    )
    FetchContent_MakeAvailable(glfw)
endif()

find_package(OpenGL REQUIRED)

# Find all C++ source files inside src and its subfolders.
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

# Add the GLAD C implementation.
add_executable(cardGame
    ${SOURCES}
    src/glad.c
)

# Header search paths.
target_include_directories(cardGame PRIVATE
    include
    src
    src/Cards
    src/Graphics
    src/Table
)

# Absolute path to shaders and textures.
target_compile_definitions(cardGame PRIVATE
    ASSET_DIR="${CMAKE_SOURCE_DIR}/"
)

# Link required libraries.
target_link_libraries(cardGame PRIVATE
    glfw
    OpenGL::GL
)

# 'dl' only exists on Linux/Mac (Windows doesn't need it)
if(UNIX)
    target_link_libraries(cardGame PRIVATE ${CMAKE_DL_LIBS})
endif()