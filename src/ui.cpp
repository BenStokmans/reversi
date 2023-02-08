#include "ui.h"

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
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
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

    if (!ImGui_ImplOpenGL3_Init("#version 150")) {
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
