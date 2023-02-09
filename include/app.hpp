#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>

#include "cameras.hpp"
#include "common.hpp"
#include "scene.hpp"
#include "shader.hpp"

#define APP_NAME "LAB4"

struct LastMouse
{
    float lastX;
    float lastY;
    bool firstMove = true;

    LastMouse(float lastX, float lastY)
    {
        this->lastX = lastX;
        this->lastY = lastY;
    }
};

class App 
{
public:
    App(int width, int height);

    void SizeCallback(int width, int height);
    void KeyCallback(int key, int scancode, int action, int mods);
    void ScrollCallback(double xoffset, double yoffset);
    void MouseMoveCallback(double xposIn, double yposIn);

    void PreLoopSetup();
    void ProcessNextFrame();

    bool ShouldClose() { return shouldClose; };

    // temp -> delete that
    glm::mat4 Proj() {return CalculateProjectionMat();};
    glm::mat4 View() {return cameras.GetCurrentViewMatrix();};

private:
    int height;
    int width;
    bool shouldClose = false;
    float lastFrameTime = 0.0f;
    float deltaFrameTime = 0.0f;
    bool wiresOn = false;

    Cameras cameras;
    LastMouse lastMouse;
    Scene scene;
    Shader* shader;
    Shader* lightShader;

    void CalculateFrameDistance();
    glm::mat4 CalculateProjectionMat();

    void SwitchWires();
};


GLFWwindow* init_window(int* width, int* height);
void register_callbacks(GLFWwindow* window, App* app);