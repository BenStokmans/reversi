#ifndef REVERSI_MAIN_H
#define REVERSI_MAIN_H

#include "helper.h"
#include <cmath>
#include <glut/glut.h>
#include "logger.h"

int main(int argc, char ** argv);
void reshapeCallback(GLsizei width, GLsizei height);
void keyBoardCallback(unsigned char key, int state, int mods);
void mouseCallback(int button, int state, int x, int y );
void render();
void closeWindow();
void timer(int value);

#endif //REVERSI_MAIN_H
