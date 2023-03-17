#ifndef REVERSI_UI_HELPER_H
#define REVERSI_UI_HELPER_H

#include "shader.h"
#include "../state.h"
#include "imgui.h"

void centeredText(const char* text);
bool centeredButton(const char* label);
GLuint createEvalVertexArray(Shader* shader);
GLuint createGridVertexArray(Shader* shader);
GLuint createDiskVertexArray(Shader* shader);
GLuint createCellVertexArray(Shader* shader);

Point screenToCellCoords(double x, double y);

#endif //REVERSI_UI_HELPER_H
