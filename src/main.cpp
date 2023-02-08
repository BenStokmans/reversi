#include "logger.h"
#include "shader.h"
#include "ui.h"
#include "state.h"
#include "shaders.h"
#include "helper.h"

int main() {
    // initialize Logger
    logger.create("TRACE");
    logger.info("starting reversi UI");

    initGLFW();
    initGLAD();
    initImGui();

    Shader gridShader = Shader(gridVert, gridFrag);
    GLuint gridVertexArray = createGridVertexArray(&gridShader);

    Shader circleShader = Shader(circleVert, circleFrag);
    GLuint diskVertexArray = createDiskVertexArray(&circleShader);

    Shader squareShader = Shader(squareVert, squareFrag);
    GLuint squareVertexArray = createSquareVertexArray(&squareShader);

    float diskSize = 2.0f / (float)boardSize;
    float squareSize = 1.0f / (float)boardSize;

    // set background color
    // rgb: 64, 83, 54
    glClearColor(0.251, 0.3255, 0.2118, 1.);

    // set shader parameters
    gridShader.set("boardSize", boardSize);
    gridShader.set("lineWidth", gridLineWidth);
    gridShader.set("lineColor", glm::vec4{0, 0, 0, 1});

    circleShader.set("radius", diskSize / 2 - diskSize / 10);
    // rgb: 6, 83, 6
    squareShader.set("color", glm::vec4{0.024, 0.3255, 0.024, 1});

    ImGuiIO& io = ImGui::GetIO(); (void)io;

    while (!glfwWindowShouldClose(glfwWindow)) {
        glClear(GL_COLOR_BUFFER_BIT);

        // start manual rendering of game ui
        // start using square shader
        squareShader.use();
        glBindVertexArray(squareVertexArray);

        for (int i = 0; i < boardSize; i++) {
            for (int j = 0; j < boardSize; j++) {
                if (!highlighted[i][j]) continue;
                float x = (-1 + squareSize) + (float)i * squareSize*2;
                float y = (-1 + squareSize) + (float)j * squareSize*2;

                squareShader.set("centre", glm::vec2(x,y));
                glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
            }
        }

        // start using grid shader
        gridShader.use();
        glBindVertexArray(gridVertexArray);
        // draw grid
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

        // start using circle shader to draw cells
        circleShader.use();
        glBindVertexArray(diskVertexArray);

        for (int i = 0; i < boardSize; i++) {
            for (int j = 0; j < boardSize; j++) {
                char side = board[i][j];
                if (side == 0) continue;
                float x = (-1 + diskSize / 2) + (float)i * diskSize;
                float y = (-1 + diskSize / 2) + (float)j * diskSize;

                bool white = side == 2;
                circleShader.set("centre", glm::vec2(x,y));
                circleShader.set("white", white);
                glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
            }
        }

        // end manual rendering game ui

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // do ImGui rendering

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
        glfwSwapBuffers(glfwWindow);

        // poll and process events
        glfwPollEvents();
    }
    logger.info("GLFW", "glfw window close requested");

    logger.trace("OPENGL", "cleaning up buffers and vertex arrays");
    glDeleteVertexArrays(1, &gridVertexArray);
    glDeleteVertexArrays(1, &diskVertexArray);

    // clean up rendering context
    deInitAll();
    return 0;
}
