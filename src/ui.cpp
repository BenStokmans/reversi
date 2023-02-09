#include "ui.h"

void drawSettingsUi() {
    ImGui::Begin("Reversi", &showGameWindow);
    if (ImGui::IsWindowFocused() && ImGui::IsKeyPressed(ImGuiKey_Q)) {
        glfwSetWindowShouldClose(glfwWindow, true);
    }
    ImGui::BeginTabBar("Reversi#TopBar", ImGuiTabBarFlags_NoTabListScrollingButtons);
    if (ImGui::BeginTabItem("Local")) {
        ImGui::Dummy(ImVec2(4,4));
        if (ImGui::Button("Restart game")) {
            initReversi();
            if (clientIsWhite) {
                Move m = getBestMove(aiDifficulty, aiColor, aiDepth);
                playMove(m);
                clientTurn = true;
                gameStarted = true;
            }
        }
        ImGui::Separator();
        if (aiManual && aiEnabled) {
            ImGui::BeginDisabled(CURRENT_PLAYER != aiColor || !gameStarted);
            if (ImGui::Button("Play next AI move")) {
                Move m = getBestMove(aiDifficulty, aiColor, aiDepth);
                playMove(m);
                clientTurn = true;
            }
            ImGui::EndDisabled();
        }
        ImGui::Checkbox("Play against computer", &aiEnabled);
        
        ImGui::BeginDisabled(!aiEnabled);
        ImGui::Checkbox("Step AI manually", &aiManual);
        ImGui::EndDisabled();

        ImGui::BeginDisabled(!aiEnabled || gameStarted);
        const char* colorItems[] = { "Black", "White" };
        if (ImGui::BeginCombo("AI Color", aiColorStr)) {
            for (int n = 0; n < IM_ARRAYSIZE(colorItems); n++)
            {
                bool selected = (aiColorStr == colorItems[n]);
                if (ImGui::Selectable(colorItems[n], selected)) {
                    aiColorStr = colorItems[n];
                    aiColor = n + 1; // NOLINT(cppcoreguidelines-narrowing-conversions)
                    clientIsWhite = aiColor == 1;
                    if (clientIsWhite && !gameStarted) {
                        clientTurn = false;
                        Move m = getBestMove(aiDifficulty, aiColor, aiDepth);
                        playMove(m);
                        clientTurn = true;
                        gameStarted = true;
                    }
                    currentLegalMoves.clear();
                }
                if (selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }

        const char* aiItems[] = { "Random", "Easy", "Normal", "Difficult" };
        if (ImGui::BeginCombo("AI difficulty", aiDifficultyStr)) {
            for (int n = 0; n < IM_ARRAYSIZE(aiItems); n++)
            {
                bool selected = (aiDifficultyStr == aiItems[n]);
                if (ImGui::Selectable(aiItems[n], selected)) {
                    aiDifficultyStr = aiItems[n];
                    aiDifficulty = static_cast<AiDifficulty>(n);
                }
                if (selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }
        ImGui::EndDisabled();

        ImGui::BeginDisabled(!aiEnabled || aiDifficulty == AiDifficulty::Random);
        ImGui::SliderInt("Depth", &aiDepth, 1, 30, "%d", ImGuiSliderFlags_AlwaysClamp);
        ImGui::EndDisabled();
        ImGui::EndTabItem();
    }
    // end local tab

    if (ImGui::BeginTabItem("Theme")) {
        ImGui::Dummy(ImVec2(4,4));
        // TODO: shorten name
        float highlightPossibleColorBuffer[4] = {
                (float)highlightPossibleColor.r / 255.f,
                (float)highlightPossibleColor.g / 255.f,
                (float)highlightPossibleColor.b / 255.f,
                (float)highlightPossibleColor.a / 255.f
        };
        ImGui::Checkbox("Highlight legal moves", &highlightPossibleSquares);
        if (ImGui::ColorEdit4("1",highlightPossibleColorBuffer, ImGuiColorEditFlags_NoLabel)) {
            highlightPossibleColor = {
                    (unsigned char)(highlightPossibleColorBuffer[0]*255.f),
                    (unsigned char)(highlightPossibleColorBuffer[1]*255.f),
                    (unsigned char)(highlightPossibleColorBuffer[2]*255.f),
                    (unsigned char)(highlightPossibleColorBuffer[3]*255.f)
            };
        }
        ImGui::Separator();
        float highlightModifiedColorBuffer[4] = {
                (float)highlightModifiedColor.r / 255.f,
                (float)highlightModifiedColor.g / 255.f,
                (float)highlightModifiedColor.b / 255.f,
                (float)highlightModifiedColor.a / 255.f
        };
        ImGui::Checkbox("Highlight modified disks", &highlightModifiedSquares);
        if (ImGui::ColorEdit4("2",highlightModifiedColorBuffer, ImGuiColorEditFlags_NoLabel)) {
            highlightModifiedColor = {
                    (unsigned char)(highlightModifiedColorBuffer[0]*255.f),
                    (unsigned char)(highlightModifiedColorBuffer[1]*255.f),
                    (unsigned char)(highlightModifiedColorBuffer[2]*255.f),
                    (unsigned char)(highlightModifiedColorBuffer[3]*255.f)
            };
        }
        ImGui::EndTabItem();
    }
    // end theme tab
    ImGui::EndTabBar();
    ImGui::End();
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
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
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

void deInitAll() {
    logger.info("IMGUI", "shutting down");
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    logger.info("GLFW", "terminating");
    if (glfwWindow != nullptr) glfwDestroyWindow(glfwWindow);
    glfwTerminate();
}
