cmake_minimum_required(VERSION 3.12)

project(reversi)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -framework OpenGL -framework GLUT")

find_package(OpenGL REQUIRED)

include_directories(${OPENGL_INCLUDE_DIRS})

add_executable(reversi WIN32 MACOSX_BUNDLE main.cpp main.h ui.cpp ui.h helper.cpp helper.h Logger.cpp Logger.h)

if (APPLE)
    set_target_properties(reversi PROPERTIES MACOSX_BUNDLE_BUNDLE_NAME "Reversi")
endif()
