#ifndef REVERSI_HELPER_H
#define REVERSI_HELPER_H

#include <vector>
#include "shader.h"
#include "state.h"

struct Point {
    int x,y;
};

GLuint createGridVertexArray(Shader* shader);
GLuint createDiskVertexArray(Shader* shader);
GLuint createSquareVertexArray(Shader* shader);

Point screenToCellCoords(double x, double y);
std::vector<Point> getSurroundingCoordinates(Point p);

#endif //REVERSI_HELPER_H
