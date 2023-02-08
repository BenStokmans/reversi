cmake_minimum_required(VERSION 3.12)

project(reversi)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")

find_package(OpenGL REQUIRED)

include_directories(${OPENGL_INCLUDE_DIRS})
include_directories(${PROJECT_SOURCE_DIR}/lib/imgui)
include_directories(${PROJECT_SOURCE_DIR}/lib/imgui/backends)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "GLFW lib only" )
set(GLFW_INSTALL OFF CACHE BOOL "GLFW lib only")

add_subdirectory(lib/glfw)
add_subdirectory(lib/glad)
add_subdirectory(lib/glm)

add_executable(reversi WIN32 MACOSX_BUNDLE src/main.cpp src/shader.cpp src/shader.h src/logger.cpp src/logger.h src/ui.cpp src/ui.h src/state.h src/input.cpp src/input.h src/state.cpp)
target_sources(reversi PRIVATE
        lib/imgui/imgui.cpp
        lib/imgui/imgui_draw.cpp
        lib/imgui/imgui_demo.cpp
        lib/imgui/imgui_tables.cpp
        lib/imgui/imgui_widgets.cpp
        lib/imgui/backends/imgui_impl_opengl3.cpp
        lib/imgui/backends/imgui_impl_glfw.cpp
        )
target_link_libraries(reversi PRIVATE glfw)
target_link_libraries(reversi PRIVATE glad)
target_link_libraries(reversi PRIVATE glm)

if (APPLE)
    set_target_properties(reversi PROPERTIES MACOSX_BUNDLE_BUNDLE_NAME "Reversi")
endif()