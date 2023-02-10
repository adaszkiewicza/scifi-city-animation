#include "model.hpp"

Material::Material(glm::vec3 diffuse, glm::vec3 specular, float shininess, glm::vec3 ambient)
 : diffuse(diffuse), specular(specular), shininess(shininess), ambient(ambient)
 {
    if (ambient == glm::vec3(0.0f))
        ambient = diffuse;

 };

void Material::SetUniforms(Shader &shader)
{
    shader.setUniform("material.ambient", ambient);
    shader.setUniform("material.diffuse", diffuse);
    shader.setUniform("material.specular", specular);
    shader.setUniform("material.shininess", shininess);
}

void Material::SetColor(glm::vec3 color)
{
    diffuse = color;
}

Model::Model(Mesh &mesh)
: mesh(mesh)
{}

void Model::SetColor(glm::vec3 color)
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

ModelStatic::ModelStatic(Mesh &mesh, Material material)
 : Model(mesh), material(material)
{};

void ModelStatic::Draw(Shader &shader)
{
    glm::mat4 model = CalculateModel();
    shader.setUniform("model", model);
    material.SetUniforms(shader);
    mesh.Draw(shader);
}

ComplexStaticModel::ComplexStaticModel(std::vector<ModelStatic*> models)
 : models(models)
 {};

void ComplexStaticModel::SetColor(glm::vec3 color)
{
    for (size_t i = 0; i < models.size(); i++)
        models[i]->SetColor(color);
}
void ComplexStaticModel::SetRotation(float angle, glm::vec3 axis)
{
    for (size_t i = 0; i < models.size(); i++)
        models[i]->SetRotation(angle, axis);
}
void ComplexStaticModel::SetNoRotation()
{
    for (size_t i = 0; i < models.size(); i++)
        models[i]->SetNoRotation();
}
void ComplexStaticModel::SetScale(float scale)
{
    for (size_t i = 0; i < models.size(); i++)
        models[i]->SetScale(scale);
}
void ComplexStaticModel::SetPlacement(glm::vec3 vector)
{
    for (size_t i = 0; i < models.size(); i++)
        models[i]->SetPlacement(vector);
}

void ComplexStaticModel::Draw(Shader& shader)
{
    for (size_t i = 0; i < models.size(); i++)
        models[i]->Draw(shader);
}

ComplexStaticModel::~ComplexStaticModel()
{
    for (size_t i = 0; i < models.size(); i++)
        delete models[i];
}