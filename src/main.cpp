#include "logger.h"
#include "shader.h"
#include "ui.h"
#include "state.h"
#include "shaders.h"

struct Point {
    float x;
    float y;
};

struct Color3 {
    float r, g, b;
};

const float gridVertices[] = {
        -1.0,  1.0, 0.0, // Top Left
        -1.0, -1.0, 0.0, // Bottom Left
        1.0, -1.0, 0.0, // Bottom Right
        1.0,  1.0, 0.0, // Top Right
};


int main()
{
    // initialize Logger
    logger.create("TRACE");
    logger.info("starting reversi UI");

    initGLFW();
    initGLAD();
    initImGui();

    Shader gridShader = Shader(gridVert, gridFrag);

    // generate vbo and buffer data
    GLuint gridVertexBuffer;
    glGenBuffers(1, &gridVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, gridVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(gridVertices), gridVertices, GL_STATIC_DRAW);

    // generate vao and configure attributes
    GLuint gridVertexArray;
    glGenVertexArrays(1, &gridVertexArray);
    glBindVertexArray(gridVertexArray);

    GLint positionIndex = gridShader.getAttributeLocation("aPosition");
    glVertexAttribPointer(positionIndex, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)nullptr);
    glEnableVertexAttribArray(positionIndex);

    Shader circleShader = Shader(circleVert, circleFrag);

    float cellSize = 2.0f / (float)boardSize;
    float cellVertices[8] = {
            cellSize, -cellSize,
            cellSize, cellSize,
            -cellSize, cellSize,
            -cellSize, -cellSize,
    };

    // generate vbo and buffer data
    GLuint cellVertexBuffer;
    glGenBuffers(1, &cellVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, cellVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cellVertices), cellVertices, GL_STATIC_DRAW);

    // generate vao and configure attributes
    GLuint cellVertexArray;
    glGenVertexArrays(1, &cellVertexArray);
    glBindVertexArray(cellVertexArray);

    // set position attribute
    positionIndex = gridShader.getAttributeLocation("aPosition");
    glVertexAttribPointer(positionIndex, 3, GL_FLOAT, GL_FALSE, 8, (void*)nullptr);
    glEnableVertexAttribArray(0);

    // setup
    glClearColor(0.251, 0.3255, 0.2118, 1.);

    // set shader parameters
    gridShader.set("boardSize", boardSize);
    gridShader.set("lineWidth", gridLineWidth);
    gridShader.set("lineColor", glm::vec4{0, 0, 0, 1});

    circleShader.set("radius", cellSize/2 - cellSize/10);

    ImGuiIO& io = ImGui::GetIO(); (void)io;

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        // start manual rendering of game ui

        // start using grid shader
        gridShader.use();
        glBindVertexArray(gridVertexArray);
        // draw grid
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

        // clear shader program and vertex array
        glBindVertexArray(0);
        glUseProgram(0);

        // start using circle shader to draw cells
        circleShader.use();
        glBindVertexArray(cellVertexArray);

        for (int i = 0; i < boardSize; i++) {
            for (int j = 0; j < boardSize; j++) {
                char side = board[i][j];
                if (side == 0) continue;
                float x = (-1+cellSize/2) + i*cellSize;
                float y = (-1+cellSize/2) + j*cellSize;

                bool white = side == 2;
                circleShader.set("centre", glm::vec2(x,y));
                circleShader.set("white", white);
                glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
            }
        }

        // unset shader program and vertex array for ImGui rendering
        glBindVertexArray(0);
        glUseProgram(0);

        // end manual rendering game ui

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // do ImGui rendering here

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }

        // swap front and back buffers to prevent tearing
        glfwSwapBuffers(window);

        // poll and process events
        glfwPollEvents();
    }
    logger.info("GLFW", "glfw window close requested");

    logger.trace("OPENGL", "cleaning up buffers and vertex arrays");
    glDeleteBuffers(1, &gridVertexBuffer);
    glDeleteBuffers(1, &cellVertexBuffer);
    glDeleteVertexArrays(1, &gridVertexArray);
    glDeleteVertexArrays(1, &cellVertexArray);

    // clean up rendering context
    deInitAll();
    return 0;
}
