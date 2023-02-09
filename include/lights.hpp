#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <string>

#include "common.hpp"
#include "shader.hpp"
#include "model.hpp"

class DirectionalLight;

class SpotLight
{
public:
    SpotLight(Mesh &mesh, glm::vec3 color, glm::vec3 position, glm::vec3 direction, float scale);
    void Draw(Shader &shader);
    void SetUniforms(Shader &shader);

private:
    Model model;
    glm::vec3 color;

    glm::vec3 position;
    glm::vec3 direction;
    const float cutOff = glm::cos(glm::radians(3.0f));
    const float outerCutOff = glm::cos(glm::radians(3.5f));

    const float constant = 1.0f;
    const float linear = 0.09f;
    const float quadratic = 0.032f;

    glm::vec3 ambient = glm::vec3(0.0f);
    glm::vec3 diffuse = glm::vec3(1.0f);
    glm::vec3 specular = glm::vec3(1.0f);
};

class PointLight
{
public:
    PointLight(Mesh &mesh, glm::vec3 color, glm::vec3 position, float scale);
    void Draw(Shader &shader);
    void SetUniforms(Shader &shader);

private:
    Model model;
    glm::vec3 color;

    glm::vec3 position;

    const float constant = 1.0f;
    const float linear = 0.09f;
    const float quadratic = 0.032f;

    glm::vec3 ambient = glm::vec3(0.1f);
    glm::vec3 diffuse = glm::vec3(0.6f);
    glm::vec3 specular = glm::vec3(1.0f);
};

class Lights
{
public:
    Lights(Mesh &defaultLightMesh);
    // pass for now
    void ProcessFrame(glm::vec3 movingObjPosition, glm::vec3 movingObjTarget);
    void SetUniforms(Shader &shader);
    void Draw(Shader &shader);

    ~Lights();

private:
    PointLight *pointLight;
    SpotLight *spotLight;
    // DirectionalLight directionalLight;
};