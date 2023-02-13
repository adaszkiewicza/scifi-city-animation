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

    ~App();

private:
    int height;
    int width;
    bool shouldClose = false;
    float lastFrameTime = 0.0f;
    float deltaFrameTime = 0.0f;
    bool wiresOn = false;
    bool fogOn = false;
    const glm::vec3 fogColor = glm::vec3(0.0f, 0.0f, 0.7f);

    Cameras cameras;
    LastMouse lastMouse;
    Scene scene;
    Shader* shaderMain;
    Shader* lightShader;
    Shader* shaderFlat;

    Shader* shader;
    bool flatOn = false;

    void CalculateFrameDistance();
    glm::mat4 CalculateProjectionMat();
    float GetFogIntensity();

    float timeFromLastPrint = 0.0f;
    uint frameCount = 0;
    const float printDistance = 1.0f;
    void PrintFPS();

    void SwitchWires();
    void SwitchFog();
    void SwitchAnimation();
    void SwitchShader();
    void SetCurrentShader();
};


GLFWwindow* init_window(int* width, int* height);
void register_callbacks(GLFWwindow* window, App* app);