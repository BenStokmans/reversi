# Reversi
## Building from source
### Prerequisites
* CMake (>= 3.12)
* Compiler with support for c++20 or greater
  * Windows: MSVC >= 1900/v143
  * Unix: GCC >= 8 or Clang >= 5
* Visual Studio **(windows only!)**
* Make **(unix)** but if you have CMake installed you probably have Make as well

### Building
* Clone the repo `git clone --recursive https://github.com/BenStokmans/reversi`
* Run `cmake .` in the root directory of the cloned repository
* On unix systems run `make`
* On windows open `reversi.sln` with visual studio and build the project `reversi` for Release.

## Dependencies
* glad (https://github.com/Dav1dde/glad)  
All the following dependencies are all included via submodules so cloning with the `--recursive` argument is required.
* GLFW (https://github.com/glfw/glfw)
* GLM (https://github.com/g-truc/glm)
* ImGui (https://github.com/ocornut/imgui)
* uint128_t (https://github.com/calccrypto/uint128_t)