#include "camera_free.hpp"

CameraFree::CameraFree(glm::vec3 position, glm::vec3 worldUp, glm::vec3 front, float fov, std::string name, float yaw, float pitch)
: BaseCamera(position, worldUp, front, fov, name), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY)
{
    Yaw = yaw;
    Pitch = pitch;

    UpdateCameraVectors();
}
  
void CameraFree::ProcessKeyboard(CameraMovement direction, float deltaTime)
{
    float velocity = MovementSpeed * deltaTime;
    if (direction == FORWARD)
        position += front * velocity;
    if (direction == BACKWARD)
        position -= front * velocity;
    if (direction == LEFT)
        position -= right * velocity;
    if (direction == RIGHT)
    position += right * velocity;

    UpdateLookAt();
}

void CameraFree::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch)
{
    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

    Yaw   += xoffset;
    Pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch) {
        if (Pitch > 89.0f)
            Pitch = 89.0f;
        if (Pitch < -89.0f)
            Pitch = -89.0f;
    }

    UpdateCameraVectors();
}

void CameraFree::ProcessMouseScroll(float yoffset)
{
    fov -= (float)yoffset;
    if (fov < 1.0f)
        fov = 1.0f;
    if (fov > 45.0f)
        fov = 45.0f;
}

void CameraFree::UpdateCameraVectors()
{
    // calculate the new front vector
    glm::vec3 tmpFront;
    tmpFront.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    tmpFront.y = sin(glm::radians(Pitch));
    tmpFront.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front = glm::normalize(tmpFront);
    // also re-calculate the right and up vector
    // normalize the vectors, because their length gets closer to 0 
    // the more you look up or down which results in slower movement.
    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));

    UpdateLookAt();
}
