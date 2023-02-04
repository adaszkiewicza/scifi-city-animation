#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "cameras.hpp"

class CameraFree : public BaseCamera
{
public:
    // euler Angles
    float Yaw;
    float Pitch;
    // camera options
    float MovementSpeed;
    float MouseSensitivity;

    // constructor with vectors
    CameraFree(glm::vec3 position, glm::vec3 worldUp, glm::vec3 front, float fov, std::string name, 
        float yaw = YAW, float pitch = PITCH);
    glm::mat4 GetViewMatrix();
    virtual void ProcessKeyboard(CameraMovement direction, float deltaTime) override;
    virtual void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true) override;
    virtual void ProcessMouseScroll(float yoffset) override;
private:
    void UpdateCameraVectors();

    // Default camera values for free camera
    static constexpr float YAW         = -90.0f;
    static constexpr float PITCH       =  0.0f;
    static constexpr float SPEED       =  2.5f;
    static constexpr float SENSITIVITY =  0.1f;
};



