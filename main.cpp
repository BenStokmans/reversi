#include "main.h"

int window;

int width = 800;
int height = 800;

int boardSize = 8;
char board[8][8];

int main(int argc, char ** argv)
{
    logger.create("DEBUG");
    logger.info("GUI", "starting gui...");

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    STREAM_TRACE("GUI", "creating window with size: " << width << "/" << height);

    glutInitWindowSize(width, height);
    if (window = glutCreateWindow("Reversi"); window == 0) {
        LOG_FATAL("window creation failed")
    }

    glutTimerFunc(10, timer, 0);

    logger.trace("GUI", "registering callback functions");

    glutDisplayFunc(render);
    glutWMCloseFunc(closeWindow);
    glutReshapeFunc(reshapeCallback);
    glutKeyboardFunc(keyBoardCallback);
    glutMouseFunc(mouseCallback);

    glClearColor(0.251, 0.3255, 0.2118, 1.);
    glEnable(GL_LINE_SMOOTH);

    logger.trace("GUI", "starting main loop");
    glutMainLoop();

    return 0;
}


void reshapeCallback(GLsizei w, GLsizei h) {
    glutReshapeWindow(width, height);
}

void keyBoardCallback(unsigned char key, int x, int y) {
    if (key == 'q') closeWindow();

    glutPostRedisplay();
}

void mouseCallback(int button, int state, int x, int y) {
    // we only want to handle left click
    if (button != 0) return;

    // 0, 0 is top left of window
    // 800, 800 is bottom right of window
    // our grid has the y-axis inverted compared to the mouse coordinates, so we'll account for that
    y = height - y;
    int cellX = floor(x / (width / boardSize));
    int cellY = floor(y / (height / boardSize));
    board[cellX][cellY] = 1;
    glutPostRedisplay();
}

void closeWindow() {
    glutDestroyWindow(window);
    exit(0);
}

void timer(int value) {
    glutPostRedisplay();
}

void render() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.0, 0.0, 0.0);

    float cellSize = 2.0f / (float)boardSize;

    // draw grid
    for (int i = 0; i < boardSize; i++) {
        drawLine(
                {-1, -1 + (float)i * cellSize},
                {1, -1 + (float)i * cellSize},
                {0, 0, 0}
                );

        drawLine(
                {-1 + (float)i * cellSize, -1},
                {-1 + (float)i * cellSize, 1},
                {0, 0, 0}
                );
    }

    for (int i = 0; i < boardSize; i++) {
        for (int j = 0; j < boardSize; j++) {
            char side = board[i][j];
            if (side == 0) continue;
            float x = (-1+cellSize/2) + i*cellSize;
            float y = (-1+cellSize/2) + j*cellSize;

            float color = 0;
            if (side == 2) color = 1;

            drawCircle({x,y}, cellSize/2 - cellSize/20, {color, color, color});
        }
    }

    glutPostRedisplay();

    glFlush();
    glutSwapBuffers();
}
