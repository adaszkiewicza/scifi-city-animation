#pragma once

#include "cameras.hpp"

class CameraStatic : public BaseCamera
{
public:
    CameraStatic(glm::vec3 position, glm::vec3 worldUp, glm::vec3 front, float fov, std::string name) 
    : BaseCamera(position, worldUp, front, fov, name) 
    {   UpdateLookAt();  };
};