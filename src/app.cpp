#include "app.hpp"
#include <cmath>

App::App(int width, int height)
: width(width), height(height), lastMouse(width / 2.0f, height / 2.0f), scene()
{
    ShaderConfig conf = ShaderConfig("shaderMain.vs", "shaderMain.fs");
    shaderMain = new Shader(conf);

    ShaderConfig lightConf = ShaderConfig("light.vs", "light.fs");
    lightShader = new Shader(lightConf);

    ShaderConfig flatConf = ShaderConfig("shaderFlat.vs", "shaderFlat.fs");
    shaderFlat = new Shader(flatConf);

    shader = shaderMain;
}

void App::SizeCallback(int width, int height)
{
    this->height = height;
    this->width = width;
    glViewport(0, 0, width, height);
}

void App::SwitchShader()
{
    if (shader == shaderMain) {
        shader = shaderFlat;
    } else if (shader == shaderFlat && !flatOn) {
        flatOn = true;
    } else {
        flatOn = false;
        shader = shaderMain;
    }
}
void App::SetCurrentShader()
{   
    if (shader == shaderMain)
        return;
    shader->use();
    if (flatOn)
        shader->setUniform("flatOn", true);
    else 
        shader->setUniform("flatOn", false);
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
    if (key == GLFW_KEY_F && action == GLFW_PRESS)
        SwitchFog();
    if (key == GLFW_KEY_A && action == GLFW_PRESS)
        SwitchAnimation();
    if (key == GLFW_KEY_N && action == GLFW_PRESS)
        scene.SetNoon();
    if (key == GLFW_KEY_M && action == GLFW_PRESS)
        scene.SetMidnight();
    if (key == GLFW_KEY_V && action == GLFW_PRESS)
        scene.SwitchVibrations();

    if (key == GLFW_KEY_1 && action == GLFW_PRESS)
        scene.ModifyCarSpotDirection(1);
    if (key == GLFW_KEY_2 && action == GLFW_PRESS)
        scene.ModifyCarSpotDirection(2);
    if (key == GLFW_KEY_3 && action == GLFW_PRESS)
        scene.ModifyCarSpotDirection(3);
    if (key == GLFW_KEY_4 && action == GLFW_PRESS)
        scene.ModifyCarSpotDirection(4);
    if (key == GLFW_KEY_5 && action == GLFW_PRESS)
        scene.ModifyCarSpotDirection(5);
    if (key == GLFW_KEY_6 && action == GLFW_PRESS)
        scene.ModifyCarSpotDirection(6);
    if (key == GLFW_KEY_7 && action == GLFW_PRESS)
        scene.ModifyCarSpotDirection(7);
    if (key == GLFW_KEY_8 && action == GLFW_PRESS)
        scene.ModifyCarSpotDirection(8);

    if (key == GLFW_KEY_S && action == GLFW_PRESS)
        SwitchShader();
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

void App::SwitchFog()
{
    fogOn = !fogOn;
}

void App::SwitchAnimation()
{
    scene.SwitchAnimation();
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
    glEnable(GL_CULL_FACE);
}

float App::GetFogIntensity()
{
    const float fogChangeSpeed = 0.05f; 
    float val =  fabs(sin(lastFrameTime * fogChangeSpeed));
    // to accentuate
    //return fmin(1.0f, val + 0.5f);
    return val;
}

void App::ProcessNextFrame()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    
    CalculateFrameDistance();
    PrintFPS();

    float fogIntensity = GetFogIntensity();

    scene.AdvanceMovement(deltaFrameTime);

    auto animationInfo = scene.GetCarPositionAndTarget();
    cameras.ProcessAnimationData(animationInfo.first, animationInfo.second);

    SetCurrentShader();

    glm::mat4 view = cameras.GetCurrentViewMatrix();
    glm::mat4 projection = CalculateProjectionMat();
    glm::vec3 position = cameras.GetCurrentCameraPosition();

    // draw scene regurally
    (*shader).use();
    (*shader).setUniform("reflection", false);
    (*shader).setUniform("view", view);
    (*shader).setUniform("projection", projection);
    (*shader).setUniform("viewPos", position);
    (*shader).setUniform("fog.fogOn", fogOn);
    (*shader).setUniform("fog.fogIntensity", fogIntensity);
    (*shader).setUniform("fog.color", fogColor);
    scene.Draw(*shader);
    
    (*lightShader).use();
    (*lightShader).setUniform("view", view);
    (*lightShader).setUniform("projection", projection);
    scene.DrawLight(*lightShader);

    // now reflection
    glEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_ALWAYS, 1, 0xFF); // Set any stencil to 1
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    glStencilMask(0xFF); // Write to stencil buffer
    glDepthMask(GL_FALSE); // Don't write to depth buffer
    glClear(GL_STENCIL_BUFFER_BIT); // Clear stencil buffer (0 by default)

    glDisable(GL_CULL_FACE);
    scene.DrawMirror(*lightShader);
    glEnable(GL_CULL_FACE);
    
    glStencilFunc(GL_EQUAL, 1, 0xFF); // Pass test if stencil value is 1
    glStencilMask(0x00); // Don't write anything to stencil buffer
    glDepthMask(GL_TRUE); // Write to depth buffer
    (*shader).use();
    (*shader).setUniform("reflection", true);
    position = glm::vec3(-1.0f * position.x, position.y, position.z);
    position += glm::vec3(-16.0f, 0.0f, 0.0f);
    (*shader).setUniform("viewPos", position);
    scene.Draw(*shader, true);
    (*lightShader).use();
    scene.DrawLight(*lightShader, true);

    glDisable(GL_STENCIL_TEST);

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

void App::PrintFPS()
{
    timeFromLastPrint += deltaFrameTime;
    frameCount++;
    if (timeFromLastPrint >= printDistance) {
        float FPS = static_cast<float>(frameCount) / timeFromLastPrint;
        frameCount = 0;
        timeFromLastPrint = 0.0f;
        std::cout << "[FPS] " << FPS << std::endl;
    }
}

App::~App()
{
    delete lightShader;
    delete shaderFlat;
    delete shaderMain;
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
