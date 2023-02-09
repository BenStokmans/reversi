#include "helper.h"

GLuint createGridVertexArray(Shader* shader) {
    const float gridVertices[12] = {
            -1.0,  1.0, 0.0,    // top left
            -1.0, -1.0, 0.0,    // bottom left
            1.0, -1.0, 0.0,     // bottom right
            1.0,  1.0, 0.0,   // top right
    };

    GLuint gridVertexBuffer;
    glGenBuffers(1, &gridVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, gridVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(gridVertices), gridVertices, GL_STATIC_DRAW);

    GLuint gridVertexArray;
    glGenVertexArrays(1, &gridVertexArray);
    glBindVertexArray(gridVertexArray);

    GLint positionIndex = shader->getAttributeLocation("aPosition");
    glVertexAttribPointer(positionIndex, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)nullptr);
    glEnableVertexAttribArray(positionIndex);

    return gridVertexArray;
}

GLuint createDiskVertexArray(Shader* shader) {
    float diskSize = 2.0f / (float)boardSize;
    float diskVertices[12] = {
            diskSize, -diskSize, 0,
            diskSize, diskSize, 0,
            -diskSize, diskSize, 0,
            -diskSize, -diskSize, 0,
    };

    GLuint diskVertexBuffer;
    glGenBuffers(1, &diskVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, diskVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(diskVertices), diskVertices, GL_STATIC_DRAW);

    GLuint diskVertexArray;
    glGenVertexArrays(1, &diskVertexArray);
    glBindVertexArray(diskVertexArray);

    GLint positionIndex = shader->getAttributeLocation("aPosition");
    glVertexAttribPointer(positionIndex, 3, GL_FLOAT, GL_FALSE, 12, (void*)nullptr);
    glEnableVertexAttribArray(positionIndex);

    return diskVertexArray;
}

GLuint createCellVertexArray(Shader* shader) {
    float cellSize = 1.0f / (float)boardSize;
    float cellVertices[12] = {
            cellSize, -cellSize, 0,
            cellSize, cellSize, 0,
            -cellSize, cellSize, 0,
            -cellSize, -cellSize, 0,
    };

    GLuint cellVertexBuffer;
    glGenBuffers(1, &cellVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, cellVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cellVertices), cellVertices, GL_STATIC_DRAW);

    GLuint cellVertexArray;
    glGenVertexArrays(1, &cellVertexArray);
    glBindVertexArray(cellVertexArray);

    GLint positionIndex = shader->getAttributeLocation("aPosition");
    glVertexAttribPointer(positionIndex, 3, GL_FLOAT, GL_FALSE, 12, (void*)nullptr);
    glEnableVertexAttribArray(positionIndex);

    return cellVertexArray;
}

Point screenToCellCoords(double x, double y) {
    // 0, 0 is top left of window
    // 800, 800 is bottom right of window
    // our grid has the y-axis inverted compared to the mouse coordinates, so we'll account for that
    y = height - y;
    int cellX = floor(x / (width / (double)boardSize));
    int cellY = floor(y / (height / (double)boardSize));
    return {cellX, cellY};
}
