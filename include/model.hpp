#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <string>

#include "common.hpp"
#include "shader.hpp"
#include "mesh.hpp"

class Model
{
public:
    Model(Mesh& mesh);
    void SetColor(glm::vec4 color);
    void SetRotation(float angle, glm::vec3 axis);
    void SetNoRotation();
    void SetScale(float scale);
    void SetPlacement(glm::vec3 vector);

    void Draw(Shader& shader);

private:
    Mesh& mesh;
    glm::vec4 color;
    glm::vec3 axis;
    glm::vec3 moveVector;
    glm::vec3 scale;
    float angle;

    glm::mat4 CalculateModel();
};