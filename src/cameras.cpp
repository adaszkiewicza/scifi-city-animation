#include "cameras.hpp"
#include "camera_free.hpp"
#include "camera_static.hpp"
#include "common.hpp"

BaseCamera::BaseCamera(glm::vec3 position, glm::vec3 worldUp, glm::vec3 front, float fov, std::string name)
{
    //if (glm::length(position) > 0.01f)
        //position = glm::normalize(position);
    this->position = position;
    this->worldUp = glm::normalize(worldUp);
    this->front = glm::normalize(front);
    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));
    this->name = name;
    this->fov = fov;
}

glm::mat4 BaseCamera::GetViewMatrix()
{
   return lookAt;
}

std::string BaseCamera::GetName()
{
    return name;
}

float BaseCamera::GetFov()
{
    return fov;
}

glm::vec3 BaseCamera::GetCameraPosition()
{
    return position;
}

void BaseCamera::UpdateLookAt()
{
    lookAt = glm::lookAt(position, position + front, up);
}

Cameras::Cameras()
{
    cameraStatic = GenerateCameraStatic();
    cameraFree = GenerateCameraFree();

    activeCamera = cameraStatic;
}

void Cameras::SwitchToNextCamera()
{
    if (activeCamera == cameraStatic)
        activeCamera = cameraFree;
    else if (activeCamera == cameraFree)
        activeCamera = cameraStatic;
}

std::string Cameras::GetCurrentCameraName()
{
    return activeCamera->GetName();
}

glm::mat4 Cameras::GetCurrentViewMatrix()
{
    return activeCamera->GetViewMatrix();
}

float Cameras::GetCurrentFov()
{
    return activeCamera->GetFov();
}

glm::vec3 Cameras::GetCurrentCameraPosition()
{
    return activeCamera->GetCameraPosition();
}

void Cameras::ProcessKeyboard(CameraMovement direction, float deltaTime)
{
    activeCamera->ProcessKeyboard(direction, deltaTime);
}

void Cameras::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch)
{
    activeCamera->ProcessMouseMovement(xoffset, yoffset, constrainPitch);
}

void Cameras::ProcessMouseScroll(float yoffset)
{
    activeCamera->ProcessMouseScroll(yoffset);
}

BaseCamera* Cameras::GenerateCameraStatic() const
{
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);
    std::string name = "Static camera";

    return new CameraStatic(position, worldUp, front, defaultFov, name);
}

BaseCamera* Cameras::GenerateCameraFree() const
{
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);
    std::string name = "Free camera";

    return new CameraFree(position, worldUp, front, defaultFov, name);
}

Cameras::~Cameras()
{
    delete cameraFree;
    delete cameraStatic;
}