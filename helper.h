//
// Created by Ben on 07/02/2023.
//

#ifndef REVERSI_HELPER_H
#define REVERSI_HELPER_H
#include <glut/glut.h>
#include <cmath>

struct Point {
    float x;
    float y;
};

struct Color3 {
    float r, g, b;
};

void drawLine(Point start, Point end, Color3 color);
void drawCircle(Point start, float radius, Color3 color);

#endif //REVERSI_HELPER_H
