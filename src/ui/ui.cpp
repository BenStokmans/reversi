#include "ui.h"

void UI::DrawUI() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    auto size = ImGui::GetMainViewport()->Size;
    ImGui::SetNextWindowSize({400,250}, ImGuiCond_Once);
    ImGui::SetNextWindowPos({size.x + 125, 125}, ImGuiCond_Once);

    ImGui::Begin("Reversi", &showGameWindow, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse);
    if (ImGui::IsWindowFocused() && ImGui::IsKeyPressed(ImGuiKey_Q)) {
        glfwSetWindowShouldClose(glfwWindow, true);
    }
    ImGui::BeginTabBar("Reversi#TopBar", ImGuiTabBarFlags_NoTabListScrollingButtons);
    if (gameOver) {
        drawGameOver();
    }

    LocalTab::Draw();
    ThemeTab::Draw();
#ifdef DEBUG
    DebugTab::Draw();
#endif

    ImGui::EndTabBar();
    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    ImGuiIO& io = ImGui::GetIO(); (void)io;
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        GLFWwindow* contextBackup = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(contextBackup);
    }
}

void UI::DrawGame(
        Shader* gridShader, GLuint gridVAO,
        Shader* cellShader, GLuint cellVAO,
        Shader* diskShader, GLuint diskVAO
    ) {
    cellShader->use();
    glBindVertexArray(cellVAO);

    // render highlighting
    highlightPossibleMoves(cellShader);
    highLightModified(cellShader);

    // draw grid on top of the highlighted cells
    gridShader->use();
    glBindVertexArray(gridVAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    // draw disks
    diskShader->use();
    glBindVertexArray(diskVAO);
    drawDisks(diskShader);
}


void glfwErrorCallback(int error, const char* description) {
    logger.error("GLFW", std::to_string(error) + ": "  + description);
}

void initGLFW() {
    glfwSetErrorCallback(glfwErrorCallback);

    if (!glfwInit()) {
        LOG_FATAL("glfw initialization failed")
    }

    logger.trace("GLFW", "glfwInit success");

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 3.2+
#ifdef __APPLE__
    logger.debug("GLFW", "enabling OpenGL forward compatibility");
    // forward compatibility is required on macOS
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    // prevent macOS resizing window
    glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_FALSE);
#endif

    glfwWindow = glfwCreateWindow(width, height, "Reversi", nullptr, nullptr);
    if (!glfwWindow) {
        glfwTerminate();
        LOG_FATAL("failed to create window")
    }
    logger.trace("GLFW", "glfwCreateWindow success");

    glfwSetWindowSizeLimits(glfwWindow, width, height, width, height);

    glfwSetFramebufferSizeCallback(glfwWindow, bufferSizeCallback);
    glfwSetKeyCallback(glfwWindow, keyCallback);
    glfwSetMouseButtonCallback(glfwWindow, mouseCallback);
    glfwSetWindowPos(glfwWindow, windowStartX, windowStartY);

    logger.trace("GLFW", "successfully set callbacks for frame buffer and HID input");


    glfwMakeContextCurrent(glfwWindow);
    glfwSwapInterval(1);

    logger.debug("GLFW", "GLFW initialization completed successfully");
}

void initGLAD() {
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        LOG_FATAL("failed to initialize OpenGL context")
    }

    logger.info("GLAD", "GLAD initialization completed successfully");
    logger.info("GLAD", "OpenGL render device: " + std::string((char*)glGetString(GL_RENDERER)));
    logger.info("GLAD", "OpenGL version: " +
            std::to_string(GLVersion.major) + "." + std::to_string(GLVersion.minor));

    // enable blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
}

void initImGui() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }
    if (!ImGui_ImplGlfw_InitForOpenGL(glfwWindow, true)) {
        LOG_FATAL("could not initialize ImGui for GLFW")
    }

    if (!ImGui_ImplOpenGL3_Init("#version 330 core")) {
        LOG_FATAL("could not initialize ImGui for OpenGL")
    }
    logger.info("IMGUI", "ImGui initialization completed successfully");
}


void UI::Init() {
    initGLFW();
    initGLAD();
    initImGui();
}


void UI::Destroy() {
    logger.info("IMGUI", "shutting down");
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    logger.info("GLFW", "terminating");
    if (glfwWindow != nullptr) glfwDestroyWindow(glfwWindow);
    glfwTerminate();
}
