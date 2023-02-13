#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include <vector>
#include <string>

class CameraStatic;
class CameraFree;

enum CameraMovement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

class BaseCamera
{
public:
    BaseCamera(glm::vec3 position, glm::vec3 worldUp, glm::vec3 front, float fov, std::string name);

    glm::mat4 GetViewMatrix();
    std::string GetName();
    float GetFov();
    glm::vec3 GetCameraPosition();

    virtual void ProcessKeyboard(CameraMovement direction, float deltaTime) {};
    virtual void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch) {};
    virtual void ProcessMouseScroll(float yoffset) {};
    virtual void ProcessAnimationData(glm::vec3 position, float target) {};

protected:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;
    float fov;

    glm::mat4 lookAt;

    std::string name;
    
    void UpdateLookAt(); 
};

class Cameras
{
public:
    Cameras();
    void SwitchToNextCamera();
    std::string GetCurrentCameraName();
    glm::mat4 GetCurrentViewMatrix();
    float GetCurrentFov();
    glm::vec3 GetCurrentCameraPosition();

    void ProcessKeyboard(CameraMovement direction, float deltaTime);
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch);
    void ProcessMouseScroll(float yoffset);

    void ProcessAnimationData(glm::vec3 position, float target);

    ~Cameras();

private:
    BaseCamera* cameraStatic;
    BaseCamera* cameraFree;
    BaseCamera* cameraFollowing;
    BaseCamera* cameraOnCar;

    BaseCamera* activeCamera;

    const float defaultFov = 45.0f;

    BaseCamera* GenerateCameraStatic() const;
    BaseCamera* GenerateCameraFree() const;
    BaseCamera* GenerateCameraFollowing() const;
    BaseCamera* GenerateCameraOnCar() const;
};