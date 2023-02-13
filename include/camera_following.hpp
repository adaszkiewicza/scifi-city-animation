#pragma once

#include "cameras.hpp"

class CameraFollowing : public BaseCamera
{
public:
    CameraFollowing(glm::vec3 position, glm::vec3 worldUp, glm::vec3 front, float fov, std::string name) 
    : BaseCamera(position, worldUp, front, fov, name) 
    {};

    virtual void ProcessAnimationData(glm::vec3 position, float angle) override
    {
        front.x = position.x;
        front.z = position.z;
        lookAt = glm::lookAt(this->position, front, worldUp);
    };
};

class CameraOnCar : public BaseCamera
{
public:
    CameraOnCar(glm::vec3 position, glm::vec3 worldUp, glm::vec3 front, float fov, std::string name) 
    : BaseCamera(position, worldUp, front, fov, name) 
    {};

    virtual void ProcessAnimationData(glm::vec3 position, float angle) override
    {
        const glm::vec3 yVector = glm::vec3(0.0f, 1.0f, 0.0f);
        glm::vec4 xVector = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
        glm::mat4 rot = glm::rotate(glm::mat4(1.0f), glm::radians(angle), yVector);
        xVector = rot * xVector;
        this->position = position;
        this->position -= 0.5f * glm::vec3(xVector);
        this->position.y += 0.3f;
        front.x = position.x + xVector.x;
        front.z = position.z + xVector.z;
        front.y = position.y - 0.1f;
        lookAt = glm::lookAt(this->position, front, worldUp);
    };
};