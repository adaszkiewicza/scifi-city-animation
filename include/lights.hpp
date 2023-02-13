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
    SpotLight(Mesh &mesh, glm::vec3 color, glm::vec3 position, glm::vec3 direction, float scale, int id=0);
    void Draw(Shader &shader, bool reflection=false);
    void SetUniforms(Shader &shader, bool reflection = false);
    void SetPositionAndTarget(glm::vec3 position, glm::vec3 target, float angle, glm::vec3 axis);

private:
    Model model;
    glm::vec3 color;

    glm::vec3 position;
    glm::vec3 direction;
    const float cutOff = glm::cos(glm::radians(12.0f));
    const float outerCutOff = glm::cos(glm::radians(13.5f));

    const float constant = 1.0f;
    const float linear = 0.09f;
    const float quadratic = 0.032f;

    glm::vec3 ambient = glm::vec3(0.0f);
    glm::vec3 diffuse = glm::vec3(1.0f);
    glm::vec3 specular = glm::vec3(1.0f);

    int id;
};

class PointLight
{
public:
    PointLight(Mesh &mesh, glm::vec3 color, glm::vec3 position, float scale, int i);
    void Draw(Shader &shader, bool reflection=false);
    void SetUniforms(Shader &shader, bool reflection = false);

private:
    int i;
    Model model;
    glm::vec3 color;

    glm::vec3 position;

    const float constant = 2.0f;
    const float linear = 0.09f;
    const float quadratic = 0.032f;

    glm::vec3 ambient = glm::vec3(0.1f);
    glm::vec3 diffuse = glm::vec3(0.6f);
    glm::vec3 specular = glm::vec3(0.2f);
};

class DirLight
{
public:
    DirLight();
    void SetUniforms(Shader &shader, bool reflection=false);
    void NewFrame(float deltaTime);
    void SetNoon();
    void SetMidnight();
private:
    glm::vec3 direction;
    
    glm::vec3 ambient = glm::vec3(0.1f);
    glm::vec3 diffuse = glm::vec3(0.6f);
    glm::vec3 specular = glm::vec3(0.2f);

    glm::vec3 colorNoon = glm::vec3(1.0f, 1.0f, 0.86f);
    glm::vec3 colorDusk = glm::vec3(1.0f, 0.729f, 0.235f);

    const float z = 0.0f;
    const float angleNoon = 90.0f;
    const float angleMidnight = 270.0f;

    const float R = 100.0f;

    float currentAngle = 0.0f;

    glm::vec3 CalculatePosition();
    glm::vec3 CalculateColor(glm::vec3 position);
};


class Lights
{
public:
    Lights(Mesh &defaultLightMesh);
    // pass for now
    //void ProcessFrame(glm::vec3 movingObjPosition, glm::vec3 movingObjTarget);
    void SetUniforms(Shader &shader, bool reflection=false);
    void Draw(Shader &shader, bool reflection=false);
    void SetNoon();
    void SetMidnight();
    void ProcessFrame(float deltaTime);
    void ProcessCarMovement(glm::vec3 position, float angle);
    void ModifySpotDirection(uint dir);

    ~Lights();

private:
    PointLight *pointLightA;
    PointLight *pointLightB;
    SpotLight *spotLight;
    DirLight *dirLight;

    const size_t offset_size = 4;
    SpotLight *spotCarA;
    int spotCarAOffset[4] = {0};
    SpotLight *spotCarB;
    int spotCarBOffset[4] = {0};

    glm::vec3 spotCarAPosition;
    glm::vec3 spotCarBPosition;
    glm::vec3 GetOffset(int* offsetAtb, glm::vec3 target);
};