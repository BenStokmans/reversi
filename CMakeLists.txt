cmake_minimum_required(VERSION 3.12)

project(reversi)

# set output name for shader resources
set(output_file "src/shaders.h")
file(WRITE ${output_file} "")
file(GLOB shaders shaders/*)
# add start include guard
file(APPEND ${output_file} "#ifndef REVERSI_RESOURCES_H\n#define REVERSI_RESOURCES_H\n\n")

foreach(shader ${shaders})
    string(REGEX MATCH "([^/]+)$" filename ${shader})
    # replace illegal characters
    string(REGEX REPLACE "\\." "" filename ${filename})
    file(READ ${shader} filedata UTF-8)
    # replace newlines
    string(REPLACE "\n" "\\n" filedata "${filedata}")
    # replace with correct capitalization for camelCase
    string(REPLACE "frag" "Frag" filename "${filename}")
    string(REPLACE "vert" "Vert" filename "${filename}")

    # add data to output file
    file(APPEND ${output_file} "const char* ${filename} = \"${filedata}\";\n")
endforeach()
# add end include guard
file(APPEND ${output_file} "\n#endif //REVERSI_RESOURCES_H")

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

add_executable(reversi WIN32 MACOSX_BUNDLE src/main.cpp src/shader.cpp src/shader.h src/logger.cpp src/logger.h src/ui.cpp src/ui.h src/state.h src/input.cpp src/input.h src/state.cpp src/shaders.h)
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