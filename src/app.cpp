#include "app.hpp"

App::App(int width, int height)
    : width(width), height(height), lastMouse(width / 2.0f, height / 2.0f) {}

void App::SizeCallback(int width, int height)
{
    this->height = height;
    this->width = width;
    glViewport(0, 0, width, height);
}

void App::KeyCallback(int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        shouldClose = true;

    if (key == GLFW_KEY_UP && action == GLFW_PRESS)
        cameras.ProcessKeyboard(FORWARD, deltaFrameTime);
    if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
        cameras.ProcessKeyboard(BACKWARD, deltaFrameTime);
    if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
        cameras.ProcessKeyboard(LEFT, deltaFrameTime);
    if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
        cameras.ProcessKeyboard(RIGHT, deltaFrameTime);

    if (key == GLFW_KEY_C && action == GLFW_PRESS)
        cameras.SwitchToNextCamera();
    if (key == GLFW_KEY_W && action == GLFW_PRESS)
        SwitchWires();
}

void App::SwitchWires()
{
    if (wiresOn) {
        wiresOn = false;
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    } else {
        wiresOn = true;
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
}

void App::ScrollCallback(double xoffset, double yoffset)
{
    cameras.ProcessMouseScroll(static_cast<float>(yoffset));
}

void App::MouseMoveCallback(double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (lastMouse.firstMove)
    {
        lastMouse.lastX = xpos;
        lastMouse.lastY = ypos;
        lastMouse.firstMove = false;
    }

    float xoffset = xpos - lastMouse.lastX;
    float yoffset = lastMouse.lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastMouse.lastX = xpos;
    lastMouse.lastY = ypos;

    cameras.ProcessMouseMovement(xoffset, yoffset, true);
}

void App::PreLoopSetup()
{
    glEnable(GL_DEPTH_TEST);
}

void App::ProcessNextFrame()
{
    CalculateFrameDistance();

    glm::mat4 view = cameras.GetCurrentViewMatrix();
    glm::mat4 projection = CalculateProjectionMat();
}

glm::mat4 App::CalculateProjectionMat()
{
    return glm::perspective(glm::radians(cameras.GetCurrentFov()), (float)width / (float)height, 0.1f, 100.0f);
}

void App::CalculateFrameDistance()
{
    float currentFrameTime = static_cast<float>(glfwGetTime());
    deltaFrameTime = currentFrameTime - lastFrameTime;
    lastFrameTime = currentFrameTime;
}

static void error_callback(int error, const char *details)
{
    print_err(std::string(details));
}

GLFWwindow *init_window(int *width, int *height)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);

    GLFWwindow *window = glfwCreateWindow((*width), (*height), APP_NAME, NULL, NULL);

    if (!window)
    {
        print_err("Failed to initialize window");
        glfwTerminate();
        return NULL;
    }

    glfwMakeContextCurrent(window);

    if (!(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)))
    {
        print_err("Failed to initialize GLAD");
        glfwTerminate();
        return NULL;
    }

    glfwGetFramebufferSize(window, width, height);
    glViewport(0, 0, (*width), (*height));

    return window;
}

void register_callbacks(GLFWwindow *window, App *app)
{
    glfwSetWindowUserPointer(window, app);

    // error
    glfwSetErrorCallback(error_callback);

    // size
    auto size_callback = [](GLFWwindow *w, int width, int height)
    {
        static_cast<App *>(glfwGetWindowUserPointer(w))->SizeCallback(width, height);
    };

    glfwSetFramebufferSizeCallback(window, size_callback);

    // key press
    auto key_callback = [](GLFWwindow *w, int key, int scancode, int action, int mods)
    {
        static_cast<App *>(glfwGetWindowUserPointer(w))->KeyCallback(key, scancode, action, mods);
    };

    glfwSetKeyCallback(window, key_callback);

    // mouse scroll
    auto scroll_callback = [](GLFWwindow *w, double xoffset, double yoffset)
    {
        static_cast<App *>(glfwGetWindowUserPointer(w))->ScrollCallback(xoffset, yoffset);
    };

    glfwSetScrollCallback(window, scroll_callback);

    // mouse move
    auto mouse_move_callback = [](GLFWwindow *w, double xposIn, double yposIn)
    {
        static_cast<App *>(glfwGetWindowUserPointer(w))->MouseMoveCallback(xposIn, yposIn);
    };

    glfwSetCursorPosCallback(window, mouse_move_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}
