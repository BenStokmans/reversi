#ifndef REVERSI_UI_H
#define REVERSI_UI_H


#define GL_SILENCE_DEPRECATION
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "imgui.h"
#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_glfw.h"

#include "logger.h"
#include "input.h"

void initGLFW();
void initGLAD();
void initImGui();
void deInitAll();

#endif //REVERSI_UI_H
