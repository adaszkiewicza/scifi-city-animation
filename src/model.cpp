#include "model.hpp"

Model::Model(Mesh& mesh)
: mesh(mesh)
{}

void Model::SetColor(glm::vec4 color)
{
    this->color = color;
}

void Model::SetRotation(float angle, glm::vec3 axis)
{
    this->angle = angle;
    this->axis = axis;
}

void Model::SetNoRotation()
{
    angle = 0;
}

void Model::SetScale(float scale)
{
    this->scale = glm::vec3(scale, scale, scale);
}

void Model::SetPlacement(glm::vec3 vector)
{
    this->moveVector = vector;
}

glm::mat4 Model::CalculateModel()
{
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::scale(model, scale);
    if (angle != 0)
        model = glm::rotate(model, glm::radians(angle), axis);
    model = glm::translate(model, moveVector);
    
    return model;
}

void Model::Draw(Shader& shader)
{
    glm::mat4 model = CalculateModel();
    shader.setUniform("model", model);
    shader.setUniform("constColor", color);

    mesh.Draw(shader);
}