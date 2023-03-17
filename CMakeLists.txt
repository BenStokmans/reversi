cmake_minimum_required(VERSION 3.19)

project(reversi C CXX)

include(CheckCCompilerFlag)

find_package(Protobuf CONFIG REQUIRED)
set(CMAKE_INCLUDE_CURRENT_DIR TRUE)

set(CMAKE_REQUIRED_LINK_OPTIONS "-arch;arm64")
check_c_compiler_flag("-arch arm64" arm64)
if (arm64)
    message("targeting arm64")
    set(CMAKE_OSX_ARCHITECTURES "arm64")
endif()

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

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -pthread")

find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})
include_directories(${PROJECT_SOURCE_DIR}/lib/imgui)
include_directories(${PROJECT_SOURCE_DIR}/lib/imgui/backends)
include_directories(${PROJECT_SOURCE_DIR}/lib/uint128_t)
include_directories(${PROJECT_SOURCE_DIR}/lib/whereami/src)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "GLFW lib only" )
set(GLFW_INSTALL OFF CACHE BOOL "GLFW lib only")

add_subdirectory(lib/glfw)
add_subdirectory(lib/glad)
add_subdirectory(lib/glm)

add_executable(reversi MACOSX_BUNDLE src/main.cpp src/ui/shader.cpp src/ui/shader.h src/logger.cpp src/logger.h src/ui/ui.cpp src/ui/ui.h src/state.h src/game/input.cpp src/game/input.h src/state.cpp src/ui/shaders.h src/ui/helper.cpp src/ui/helper.h src/game/reversi.cpp src/game/reversi.h src/game/ai.cpp src/game/ai.h src/main.h src/ui/tabs/game.cpp src/ui/tabs/game.h src/ui/tabs/theme.cpp src/ui/tabs/theme.h src/ui/tabs/debug.cpp src/ui/tabs/debug.h src/ui/game.h src/ui/game.cpp src/ui/shaders.cpp src/ui/tabs/ai.cpp src/ui/tabs/ai.h src/game/fastboard/fastboard.cpp src/game/fastboard/fastboard.h src/game/fastboard/edges.cpp src/game/fastboard/edges.h src/game/fastboard/flip.cpp src/game/fastboard/flip.h src/game/fastboard/moves.cpp src/game/fastboard/moves.h src/online/client.cpp src/online/client.h src/game/search.cpp src/game/search.h src/online/reversi.proto src/ui/tabs/online.cpp src/ui/tabs/online.h src/online/handler.cpp src/online/handler.h)
target_sources(reversi PRIVATE
        lib/imgui/imgui.cpp
        lib/imgui/imgui_draw.cpp
        lib/imgui/imgui_demo.cpp
        lib/imgui/imgui_tables.cpp
        lib/imgui/imgui_widgets.cpp
        lib/imgui/backends/imgui_impl_opengl3.cpp
        lib/imgui/backends/imgui_impl_glfw.cpp
        )
target_sources(reversi PRIVATE lib/uint128_t/uint128_t.cpp)
target_sources(reversi PRIVATE lib/whereami/src/whereami.c)

target_link_libraries(reversi PRIVATE glfw)
target_link_libraries(reversi PRIVATE glad)
target_link_libraries(reversi PRIVATE glm)
target_link_libraries(reversi PRIVATE protobuf::libprotobuf)
protobuf_generate(TARGET reversi)

if (APPLE)
    set_target_properties(reversi PROPERTIES MACOSX_BUNDLE_BUNDLE_NAME "Reversi")
endif()