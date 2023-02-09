#include "lights.hpp"

SpotLight::SpotLight(Mesh &mesh, glm::vec3 color,glm::vec3 position, glm::vec3 direction, float scale)
 : model(mesh), color(color), position(position), direction(direction)
{
    model.SetColor(color);
    model.SetNoRotation();
    model.SetPlacement(position);
    model.SetScale(scale);
    
    ambient = color * ambient;
    diffuse = color * diffuse;
    specular = color * specular;
}

void SpotLight::Draw(Shader &shader)
{
    model.Draw(shader);
}

void SpotLight::SetUniforms(Shader &shader)
{
    shader.setUniform("spotLight.position", position);
    shader.setUniform("spotLight.direction", direction);

    shader.setUniform("spotLight.ambient", ambient);
    shader.setUniform("spotLight.diffuse", diffuse);
    shader.setUniform("spotLight.specular", specular);

    shader.setUniform("spotLight.constant", constant);
    shader.setUniform("spotLight.linear", linear);
    shader.setUniform("spotLight.quadratic", quadratic);

    shader.setUniform("spotLight.cutOff", cutOff);
    shader.setUniform("spotLight.outerCutOff", outerCutOff);
}

PointLight::PointLight(Mesh &mesh, glm::vec3 color, glm::vec3 position, float scale)
 : model(mesh), color(color), position(position)
{
    model.SetColor(color);
    model.SetNoRotation();
    model.SetPlacement(position);
    model.SetScale(scale);
    
    ambient = color * ambient;
    diffuse = color * diffuse;
    specular = color * specular;
}

void PointLight::Draw(Shader &shader)
{
    model.Draw(shader);
}

void PointLight::SetUniforms(Shader &shader)
{
    shader.setUniform("pointLight.position", position);

    shader.setUniform("pointLight.ambient", ambient);
    shader.setUniform("pointLight.diffuse", diffuse);
    shader.setUniform("pointLight.specular", specular);

    shader.setUniform("pointLight.constant", constant);
    shader.setUniform("pointLight.linear", linear);
    shader.setUniform("pointLight.quadratic", quadratic);
}

Lights::Lights(Mesh &defaultLightMesh)
{
    glm::vec3 lightColor, lightDirection, lightPosition;
    float scale;

    lightColor = glm::vec3(1.0f, 1.0f, 0.0f);
    lightPosition = glm::vec3(-6.0f, -1.0f, 3.0f);
    scale = 0.1f;
    pointLight = new PointLight(defaultLightMesh, lightColor, lightPosition, scale);

    lightColor = glm::vec3(1.0f, 0.0f, 0.9f);
    lightPosition = glm::vec3(0.0f, 0.0f, 2.0f);
    lightDirection = glm::vec3(0.0f, 0.0f, -1.0f);
    scale = 0.1f;
    spotLight = new SpotLight(defaultLightMesh, lightColor, lightPosition, lightDirection, scale);
}

void Lights::ProcessFrame(glm::vec3 movingObjPosition, glm::vec3 movingObjTarget)
{
    // pass for now:/
}

void Lights::SetUniforms(Shader &shader)
{
    pointLight->SetUniforms(shader);
    spotLight->SetUniforms(shader);
}

void Lights::Draw(Shader &shader)
{
    pointLight->Draw(shader);
    spotLight->Draw(shader);
}

Lights::~Lights()
{
    delete pointLight;
    delete spotLight;
}


