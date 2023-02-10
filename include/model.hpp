#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include <vector>
#include <string>

#include "common.hpp"
#include "shader.hpp"
#include "mesh.hpp"


class Material
{
public:
    Material(glm::vec3 diffuse, glm::vec3 specular, float shininess, glm::vec3 ambient = glm::vec3(0.0f));

    void SetUniforms(Shader &shader);
    void SetColor(glm::vec3 color);
private:
    float shininess;
    glm::vec3 diffuse;
    glm::vec3 specular;
    glm::vec3 ambient;
};

class Model
{
public:
    Model(Mesh& mesh);
    void SetColor(glm::vec3 color);
    void SetRotation(float angle, glm::vec3 axis);
    void SetNoRotation();
    void SetScale(float scale);
    void SetPlacement(glm::vec3 vector);

    void Draw(Shader& shader);

protected:
    Mesh& mesh;
    glm::vec3 color;
    glm::vec3 axis;
    glm::vec3 moveVector;
    glm::vec3 scale;
    float angle;

    glm::mat4 CalculateModel();
};

class ModelStatic : public Model
{
public:
    ModelStatic(Mesh &mesh, Material material);

    void Draw(Shader &shader);

private:
    Material material;
};

class ComplexStaticModel
{
public:
    ComplexStaticModel(std::vector<ModelStatic*> models);
    void SetColor(glm::vec3 color);
    void SetRotation(float angle, glm::vec3 axis);
    void SetNoRotation();
    void SetScale(float scale);
    void SetPlacement(glm::vec3 vector);

    void Draw(Shader& shader);

    ~ComplexStaticModel();

private:
    std::vector<ModelStatic*> models;
};