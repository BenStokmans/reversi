#include "helper.h"

GLuint createGridVertexArray(Shader *shader) {
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

    // generate vao and configure attributes
    GLuint gridVertexArray;
    glGenVertexArrays(1, &gridVertexArray);
    glBindVertexArray(gridVertexArray);

    GLint positionIndex = shader->getAttributeLocation("aPosition");
    glVertexAttribPointer(positionIndex, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)nullptr);
    glEnableVertexAttribArray(positionIndex);

    return gridVertexArray;
}

GLuint createDiskVertexArray(Shader *shader) {
    float diskSize = 2.0f / (float)boardSize;
    float diskVertices[12] = {
            diskSize, -diskSize, 0,
            diskSize, diskSize, 0,
            -diskSize, diskSize, 0,
            -diskSize, -diskSize, 0,
    };

    // generate vbo and buffer data
    GLuint diskVertexBuffer;
    glGenBuffers(1, &diskVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, diskVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(diskVertices), diskVertices, GL_STATIC_DRAW);

    // generate vao and configure attributes
    GLuint diskVertexArray;
    glGenVertexArrays(1, &diskVertexArray);
    glBindVertexArray(diskVertexArray);

    // set position attribute
    GLint positionIndex = shader->getAttributeLocation("aPosition");
    glVertexAttribPointer(positionIndex, 3, GL_FLOAT, GL_FALSE, 12, (void*)nullptr);
    glEnableVertexAttribArray(positionIndex);

    return diskVertexArray;
}

GLuint createSquareVertexArray(Shader *shader) {
    float squareSize = 1.0f / (float)boardSize;
    float squareVertices[12] = {
            squareSize, -squareSize, 0,
            squareSize, squareSize, 0,
            -squareSize, squareSize, 0,
            -squareSize, -squareSize, 0,
    };

    // generate vertex buffer and buffer data
    GLuint squareVertexBuffer;
    glGenBuffers(1, &squareVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, squareVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(squareVertices), squareVertices, GL_STATIC_DRAW);

    // generate vertex array and configure attributes
    GLuint squareVertexArray;
    glGenVertexArrays(1, &squareVertexArray);
    glBindVertexArray(squareVertexArray);

    // set position attribute
    GLint positionIndex = shader->getAttributeLocation("aPosition");
    glVertexAttribPointer(positionIndex, 3, GL_FLOAT, GL_FALSE, 12, (void*)nullptr);
    glEnableVertexAttribArray(positionIndex);

    return squareVertexArray;
}
