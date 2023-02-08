#ifndef REVERSI_HELPER_H
#define REVERSI_HELPER_H


#include "shader.h"
#include "state.h"

GLuint createGridVertexArray(Shader *shader);
GLuint createDiskVertexArray(Shader *shader);
GLuint createSquareVertexArray(Shader* shader);

#endif //REVERSI_HELPER_H
