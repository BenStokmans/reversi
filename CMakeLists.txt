cmake_minimum_required(VERSION 3.12)

project(reversi)

# set output name for shader resources
set(output_header "src/ui/shaders.h")
set(output_source "src/ui/shaders.cpp")

# clear both files
file(WRITE ${output_header} "")
file(WRITE ${output_source} "")

file(GLOB shaders shaders/*)

# add start include guard
file(APPEND ${output_header} "#ifndef REVERSI_RESOURCES_H\n#define REVERSI_RESOURCES_H\n\n")
# include header
file(APPEND ${output_source} "#include \"shaders.h\"\n\n")

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
    file(APPEND ${output_source} "const char* ${filename} = \"${filedata}\";\n")
    file(APPEND ${output_header} "extern const char* ${filename};\n")

endforeach()
# add end include guard
file(APPEND ${output_header} "\n#endif //REVERSI_RESOURCES_H")


if (MSVC_VERSION GREATER_EQUAL "1900")
    include(CheckCXXCompilerFlag)
    CHECK_CXX_COMPILER_FLAG("/std:c++latest" _cpp_latest_flag_supported)
    if (_cpp_latest_flag_supported)
        add_compile_options("/std:c++latest")
    endif()
endif()
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

add_executable(reversi WIN32 MACOSX_BUNDLE src/main.cpp src/ui/shader.cpp src/ui/shader.h src/logger.cpp src/logger.h src/ui/ui.cpp src/ui/ui.h src/state.h src/game/input.cpp src/game/input.h src/state.cpp src/ui/shaders.h src/ui/helper.cpp src/ui/helper.h src/game/reversi.cpp src/game/reversi.h src/game/ai.cpp src/game/ai.h src/main.h src/ui/tabs/local.cpp src/ui/tabs/local.h src/ui/tabs/theme.cpp src/ui/tabs/theme.h src/ui/tabs/debug.cpp src/ui/tabs/debug.h src/ui/game.h src/ui/game.cpp src/ui/shaders.cpp src/game/board.cpp src/game/board.h src/ui/tabs/ai.cpp src/ui/tabs/ai.h)
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