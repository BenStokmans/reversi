#include "main.h"

int main() {
    // initialize Logger
    logger.create("TRACE");
    logger.info("starting reversi UI");

    UI::Init();
    Game::Init();

    LOG_SHADER_NAME(gridVert, gridFrag);
    Shader gridShader = Shader(gridVert, gridFrag);
    GLuint gridVertexArray = createGridVertexArray(&gridShader);

    LOG_SHADER_NAME(circleVert, circleFrag);
    Shader circleShader = Shader(circleVert, circleFrag);
    GLuint diskVertexArray = createDiskVertexArray(&circleShader);

    LOG_SHADER_NAME(squareVert, squareFragq);
    Shader squareShader = Shader(squareVert, squareFrag);
    GLuint squareVertexArray = createSquareVertexArray(&squareShader);

    float diskSize = 2.0f / (float)boardSize;

    // set background color
    // rgb: 64, 83, 54
    glClearColor(0.251, 0.3255, 0.2118, 1.);

    // set shader parameters
    gridShader.set("boardSize", boardSize);
    gridShader.set("lineWidth", gridLineWidth);
    gridShader.set("lineColor", glm::vec4{0, 0, 0, 1});

    circleShader.set("radius", diskSize / 2 - diskSize / 10);

    ImGuiIO& io = ImGui::GetIO(); (void)io;
    while (!glfwWindowShouldClose(glfwWindow)) {
        glClear(GL_COLOR_BUFFER_BIT);

        UI::DrawGame(
                &gridShader, gridVertexArray,
                &squareShader, squareVertexArray,
                &circleShader, diskVertexArray
                     );

        UI::DrawUI();
        // swap front and back buffers to prevent tearing
        glfwSwapBuffers(glfwWindow);

        // poll and process events
        glfwPollEvents();
    }
    logger.info("GLFW", "glfw window close requested");

    logger.trace("OPENGL", "cleaning up buffers and vertex arrays");

    // clean up rendering context
    UI::Destroy();
    return 0;
}
