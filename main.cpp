#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include <iostream>
#include <math.h>

#include "shader.hpp"
#include "app.hpp"
#include "common.hpp"

#define INIT_WIDTH 100
#define INIT_HEIGHT 100

int main(int argc, char **argv)
{
    int windowWidth = INIT_WIDTH, windowHeight = INIT_HEIGHT; 
    GLFWwindow* window;
    window = init_window(&windowWidth, &windowHeight);

    if (!window)
        return -1;

    App* app = new App(windowWidth, windowHeight);
    register_callbacks(window, app);
    app->PreLoopSetup();

    // event loop
    while (!glfwWindowShouldClose(window))
    {
        app->ProcessNextFrame();
        glfwSwapBuffers(window);
        glfwPollEvents();

        if(app->ShouldClose())
            glfwSetWindowShouldClose(window, true);
    }

    delete app;
    glfwTerminate();

    return 0;
}