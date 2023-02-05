cmake_minimum_required(VERSION 3.1)

project(reversi)

find_package(OpenGL REQUIRED)

include_directories(${OPENGL_INCLUDE_DIRS})

set(GLFW_BUILD_DOCS OFF CACHE BOOL "GLFW lib only" )
set(GLFW_INSTALL OFF CACHE BOOL "GLFW lib only")

add_subdirectory(glfw)

add_executable(reversi WIN32 MACOSX_BUNDLE main.cpp)
target_link_libraries(reversi ${OPENGL_LIBRARIES} glfw)

if (APPLE)
    set_target_properties(reversi PROPERTIES MACOSX_BUNDLE_BUNDLE_NAME "Reversi")
endif()
