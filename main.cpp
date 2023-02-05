#include <GLFW/glfw3.h>

int main()
{
    GLFWwindow* window;

    if (!glfwInit())
    {
        // glfw initialization failed exit
        return -1;
    }

    window = glfwCreateWindow(640, 480, "Reversi", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glClearColor(0.4f, 0.3f, 0.4f, 0.0f);

    while (!glfwWindowShouldClose(window))
    {
        // do rendering
        glClear(GL_COLOR_BUFFER_BIT);

        // swap front and back buffers to prevent tearing
        glfwSwapBuffers(window);

        // poll and process events
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
