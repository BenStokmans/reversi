#ifndef REVERSI_INPUT_H
#define REVERSI_INPUT_H

#include <cmath>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "logger.h"
#include "state.h"
#include "imgui.h"

void bufferSizeCallback([[maybe_unused]] GLFWwindow* win, int w, int h);
void keyCallback(GLFWwindow* window, int key, [[maybe_unused]] int scancode, [[maybe_unused]] int action, int mods);
void mouseCallback(GLFWwindow* window, int button, [[maybe_unused]] int action, [[maybe_unused]] int mods);

#endif //REVERSI_INPUT_H
