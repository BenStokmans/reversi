#include "main.h"

int main() {
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    // initialize Logger
    logger.create("TRACE");
    logger.info("starting reversi UI");

    UI::Init();
    AIEnv::Init();
    Game::Init();
    Client::Connect();


    // shader loading has to be done in main
    LOG_SHADER_NAME(evalVert, evalFrag);
    Shader evalShader = Shader(evalVert, evalFrag);
    GLuint evalVertexArray = createEvalVertexArray(&evalShader);

    LOG_SHADER_NAME(gridVert, gridFrag);
    Shader gridShader = Shader(gridVert, gridFrag);
    GLuint gridVertexArray = createGridVertexArray(&gridShader);

    LOG_SHADER_NAME(cellVert, cellFrag);
    Shader cellShader = Shader(cellVert, cellFrag);
    GLuint cellVertexArray = createCellVertexArray(&cellShader);

    LOG_SHADER_NAME(diskVert, diskFrag);
    Shader diskShader = Shader(diskVert, diskFrag);
    GLuint diskVertexArray = createDiskVertexArray(&diskShader);

    float diskSize = 2.0f / (float)boardSize;

    // set background color
    // rgb: 64, 83, 54
    glClearColor(0.251, 0.3255, 0.2118, 1.);

    // set shader parameters
    gridShader.set("boardSize", boardSize);
    gridShader.set("lineWidth", gridLineWidth);
    gridShader.set("lineColor", glm::vec4{0, 0, 0, 1});

    diskShader.set("radius", diskSize / 2 - diskSize / 10);

    ImGuiIO& io = ImGui::GetIO(); (void)io;
    while (!glfwWindowShouldClose(glfwWindow)) {
        glClear(GL_COLOR_BUFFER_BIT);

        UI::DrawGame(
                &evalShader, evalVertexArray,
                &gridShader, gridVertexArray,
                &cellShader, cellVertexArray,
                &diskShader, diskVertexArray
                     );

        UI::DrawUI();
        // swap front and back buffers to prevent tearing
        glfwSwapBuffers(glfwWindow);

        // poll and process events
        glfwPollEvents();
    }
    glDeleteVertexArrays(1, &gridVertexArray);
    glDeleteVertexArrays(1, &cellVertexArray);
    glDeleteVertexArrays(1, &diskVertexArray);
    logger.info("GLFW", "glfw window close requested");

    logger.trace("OPENGL", "cleaning up buffers and vertex arrays");

    // clean up rendering context
    UI::Destroy();
    return 0;
}
