#ifndef REVERSI_HELPER_H
#define REVERSI_HELPER_H

#include "shader.h"
#include "state.h"

GLuint createGridVertexArray(Shader* shader);
GLuint createDiskVertexArray(Shader* shader);
GLuint createSquareVertexArray(Shader* shader);

Point screenToCellCoords(double x, double y);
std::vector<Point> getSurroundingCoordinates(Point p);

#endif //REVERSI_HELPER_H
