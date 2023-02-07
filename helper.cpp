//
// Created by Ben on 07/02/2023.
//

#include "helper.h"

void drawLine(Point start, Point end, Color3 color) {
    glBegin(GL_LINES);
    glColor3f(color.r, color.g, color.b);

    glVertex2f(start.x, start.y);
    glVertex2f(end.x, end.y);
    glEnd();
}

void drawCircle(Point center, float radius, Color3 color) {
    glBegin(GL_TRIANGLE_FAN);
    glColor3f(color.r, color.g, color.b);

    glVertex2f(center.x, center.y);
    for(int i = 0; i <= 360; i++)
        glVertex2f(radius*cos(M_PI * i / 180.0) + center.x, radius*sin(M_PI * i / 180.0) + center.y); // NOLINT(cppcoreguidelines-narrowing-conversions)

    glEnd();
}