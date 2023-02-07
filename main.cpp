#define GL_SILENCE_DEPRECATION
#define GLM_FORCE_CTOR_INIT

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

#include "Logger.h"
#include "Shader.h"


const float vertices[] = {
        -1.0,  1.0, 0.0, // Top Left
        -1.0, -1.0, 0.0, // Bottom Left
        1.0, -1.0, 0.0, // Bottom Right
        1.0,  1.0, 0.0, // Top Right
};

void bufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE || key == GLFW_KEY_Q || (key == GLFW_KEY_F4 && GLFW_MOD_ALT & mods)) {
        glfwSetWindowShouldClose(window, true);
    }
}

void mouseCallback(GLFWwindow* window, int button, int action, int mods)
{
    // process
}


int main()
{
    // initialize Logger
    logger.create("TRACE");

    logger.info("starting reversi UI");
    GLFWwindow* window;

    if (!glfwInit()) {
        LOG_FATAL("glfw initialization failed")
    }

    logger.trace("GUI", "glfwInit success");

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    logger.debug("GUI", "enabling opengl forward compatability...");
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    window = glfwCreateWindow(800, 800, "Reversi", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        LOG_FATAL("failed to create window")
    }
    logger.trace("GUI", "glfwCreateWindow success");

    glfwSetFramebufferSizeCallback(window, bufferSizeCallback);
    glfwSetKeyCallback(window, keyCallback);
    glfwSetMouseButtonCallback(window, mouseCallback);
    logger.trace("GUI", "successfully set callbacks for frame buffer and HID input");

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        LOG_FATAL("failed to initialize OpenGL context")
    }

    Shader shader = Shader::fromFile("shaders/grid");

    // generate vbo and buffer data
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // generate vao and configure attributes
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLint positionIndex = shader.getAttributeLocation("aPosition");
    glVertexAttribPointer(positionIndex, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)nullptr);
    glEnableVertexAttribArray(positionIndex);

    // setup
    glClearColor(0.2f, 0.3f, 0.2f, 1.0f);
    shader.set("transform", scale(glm::mat4(), glm::vec3{ 1.f, 1.f, 1.f }));

    while (!glfwWindowShouldClose(window)) {
        glBindVertexArray(vao);
        shader.use();

        glClear(GL_COLOR_BUFFER_BIT);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

        // swap front and back buffers to prevent tearing
        glfwSwapBuffers(window);

        // poll and process events
        glfwPollEvents();
    }
    logger.info("GUI", "glfw window close requested; terminating...");

    logger.trace("GUI", "cleaning up buffers and vertex arrays");
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
    glfwDestroyWindow(window);

    glfwTerminate();
    return 0;
}
