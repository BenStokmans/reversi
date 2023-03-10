#ifndef REVERSI_UI_HELPER_H
#define REVERSI_UI_HELPER_H

#include "shader.h"
#include "../state.h"

GLuint createGridVertexArray(Shader* shader);
GLuint createDiskVertexArray(Shader* shader);
GLuint createCellVertexArray(Shader* shader);

Point screenToCellCoords(double x, double y);

#endif //REVERSI_UI_HELPER_H
