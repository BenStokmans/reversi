cmake_minimum_required(VERSION 3.12)

project(reversi)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")

find_package(OpenGL REQUIRED)

include_directories(${OPENGL_INCLUDE_DIRS})
include_directories(${PROJECT_SOURCE_DIR}/include/imgui)
include_directories(${PROJECT_SOURCE_DIR}/include/imgui/backends)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "GLFW lib only" )
set(GLFW_INSTALL OFF CACHE BOOL "GLFW lib only")

add_subdirectory(include/glfw)
add_subdirectory(include/glad)
add_subdirectory(include/glm)

add_executable(reversi WIN32 MACOSX_BUNDLE main.cpp shader.cpp shader.h logger.cpp logger.h ui.cpp ui.h state.h input.cpp input.h state.cpp)
target_sources(reversi PRIVATE
        include/imgui/imgui.cpp
        include/imgui/imgui_draw.cpp
        include/imgui/imgui_demo.cpp
        include/imgui/imgui_tables.cpp
        include/imgui/imgui_widgets.cpp
        include/imgui/backends/imgui_impl_opengl3.cpp
        include/imgui/backends/imgui_impl_glfw.cpp
        )
target_link_libraries(reversi PRIVATE glfw)
target_link_libraries(reversi PRIVATE glad)
target_link_libraries(reversi PRIVATE glm)

if (APPLE)
    set_target_properties(reversi PROPERTIES MACOSX_BUNDLE_BUNDLE_NAME "Reversi")
endif()