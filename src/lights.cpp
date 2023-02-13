#include "lights.hpp"

SpotLight::SpotLight(Mesh &mesh, glm::vec3 color,glm::vec3 position, glm::vec3 direction, float scale, int id)
 : model(mesh), color(color), position(position), direction(direction), id(id)
{
    model.SetColor(color);
    model.SetNoRotation();
    model.SetPlacement(position);
    model.SetScale(scale);
    
    ambient = ambient * color;
    diffuse = diffuse * color;
    specular = specular * color;
}

void SpotLight::Draw(Shader &shader, bool reflection)
{
    model.Draw(shader, reflection);
}

void SpotLight::SetPositionAndTarget(glm::vec3 position, glm::vec3 target, float angle, glm::vec3 axis)
{
    this->position = position;
    this->direction = target;
    model.SetPlacement(position);
    model.SetRotation(angle, axis);
}

void SpotLight::SetUniforms(Shader &shader, bool reflection)
{
    std::string text = "spotLight[" + std::to_string(id) + "]";
    glm::vec3 tempPos = position;
    glm::vec3 tempDir = direction; 
    if (reflection) {
        tempPos = glm::vec3(-1 * tempPos.x, tempPos.y, tempPos.z);
        tempPos += glm::vec3(-16.0f, 0.0f, 0.0f);
    }
    shader.setUniform(text + ".position", tempPos);
    if (reflection) {
        tempDir = glm::vec3(-1 * tempDir.x, tempDir.y, tempDir.z);
        tempDir += glm::vec3(-16.0f, 0.0f, 0.0f);
    }
    shader.setUniform(text + ".direction", tempDir);

    shader.setUniform(text + ".ambient", ambient);
    shader.setUniform(text + ".diffuse", diffuse);
    shader.setUniform(text + ".specular", specular);

    shader.setUniform(text + ".constant", constant);
    shader.setUniform(text + ".linear", linear);
    shader.setUniform(text + ".quadratic", quadratic);

    shader.setUniform(text + ".cutOff", cutOff);
    shader.setUniform(text + ".outerCutOff", outerCutOff);
}

PointLight::PointLight(Mesh &mesh, glm::vec3 color, glm::vec3 position, float scale, int i)
 : model(mesh), color(color), position(position), i(i)
{
    model.SetColor(color);
    model.SetNoRotation();
    model.SetPlacement(position);
    model.SetScale(scale);
    
    ambient = ambient * color;
    diffuse = diffuse * color;
    specular = specular * color;
}

void PointLight::Draw(Shader &shader, bool reflection)
{
    model.Draw(shader, reflection);
}

void PointLight::SetUniforms(Shader &shader, bool reflection)
{
    glm::vec3 tempPos = position;
    if (reflection) {
        tempPos = glm::vec3(-1 * tempPos.x, tempPos.y, tempPos.z);
        tempPos += glm::vec3(-16.0f, 0.0f, 0.0f);
    }
    std::string text = "pointLight[" + std::to_string(i) + "]";
    shader.setUniform(text + ".position", tempPos);

    shader.setUniform(text + ".ambient", ambient);
    shader.setUniform(text + ".diffuse", diffuse);
    shader.setUniform(text + ".specular", specular);

    shader.setUniform(text + ".constant", constant);
    shader.setUniform(text + ".linear", linear);
    shader.setUniform(text + ".quadratic", quadratic);
}

DirLight::DirLight(){}

void DirLight::SetUniforms(Shader &shader, bool reflection)
{
    glm::vec3 position = CalculatePosition();
    glm::vec3 color = CalculateColor(position);
    //std::cout << "(x, y)" << position.x<< " " << position.y<< " angle: " << currentAngle<<std::endl;
    //std::cout << "color: " <<glm::to_string(color) <<std::endl;
    glm::vec3 tempPos = position;
    if (reflection) {
        tempPos = glm::vec3(-1 * tempPos.x, tempPos.y, tempPos.z);
        tempPos += glm::vec3(-16.0f, 0.0f, 0.0f);
    }

    glm::vec3 tmpAmbient = ((position.y / this->R) * ambient) * color;
    glm::vec3 tmpDiffuse = ((position.y / this->R) * diffuse) * color;
    glm::vec3 tmpSpecular = ((position.y / this->R) * specular) * color;

    shader.setUniform("dirLight.direction", tempPos);

    shader.setUniform("dirLight.ambient", tmpAmbient);
    shader.setUniform("dirLight.diffuse", tmpDiffuse);
    shader.setUniform("dirLight.specular", tmpSpecular);
}

glm::vec3 DirLight::CalculateColor(glm::vec3 position)
{
    if (position.y <= 0) 
        return glm::vec3(0.0f);
    
    float R = colorNoon.x;
    float ratio = position.y / this->R;
    float G = (colorNoon.y - colorDusk.y) * ratio + colorDusk.y;
    float B = (colorNoon.z - colorDusk.z) * ratio + colorDusk.z;

    return glm::vec3(R, G, B); 
}

glm::vec3 DirLight::CalculatePosition()
{
    float rad = glm::radians(currentAngle);
    float x = cos(rad) * R;
    float y = sin(rad) * R;

    return glm::vec3(x, y, z);
}

void DirLight::NewFrame(float deltaTime)
{   
    const float angleSpeed = 5.0f;
    float deltaAngle = angleSpeed * deltaTime;
    currentAngle += deltaAngle;
    if (currentAngle >= 360.0f)
        currentAngle -= 360.0f;
}

void DirLight::SetMidnight()
{
    currentAngle = angleMidnight;
}

void DirLight::SetNoon()
{
    currentAngle = angleNoon;
}


Lights::Lights(Mesh &defaultLightMesh)
{
    glm::vec3 lightColor, lightDirection, lightPosition;
    float scale;

    lightColor = glm::vec3(1.0f, 0.7f, 0.4f);
    lightPosition = glm::vec3(0.0f, 0.85f, -0.7f);
    scale = 0.03f;
    pointLightA = new PointLight(defaultLightMesh, lightColor, lightPosition, scale, 0);

    lightColor = glm::vec3(0.2f, 0.2f, 0.9f);
    lightPosition = glm::vec3(1.55f, 1.55f, -3.3f);
    scale = 0.03f;
    pointLightB = new PointLight(defaultLightMesh, lightColor, lightPosition, scale, 1);

    lightColor = glm::vec3(1.0f, 0.2f, 0.2f);
    lightPosition = glm::vec3(-3.9f, 2.0f, -0.3f);
    lightDirection = glm::vec3(1.0f, 0.0f, 0.0f);
    scale = 0.03f;
    spotLight = new SpotLight(defaultLightMesh, lightColor, lightPosition, lightDirection, scale, 0);

    dirLight = new DirLight();

    lightColor = glm::vec3(0.717f, 0.760f, 0.019f);
    scale = 0.02f;
    spotCarAPosition = glm::vec3(0.03f, 0.0f, 0.0f);
    lightDirection = glm::vec3(0.0f, 0.0f, 0.0f);
    spotCarA = new SpotLight(defaultLightMesh, lightColor, spotCarAPosition, lightDirection, scale, 1);
    lightColor = glm::vec3(0.717f, 0.760f, 0.619f);
    spotCarBPosition = glm::vec3(-0.05f, 0.0f, 0.0f);
    spotCarB = new SpotLight(defaultLightMesh, lightColor, spotCarBPosition, lightDirection, scale, 2);
}


void Lights::SetUniforms(Shader &shader, bool reflection)
{
    pointLightA->SetUniforms(shader, reflection);
    pointLightB->SetUniforms(shader, reflection);
    spotLight->SetUniforms(shader, reflection);
    dirLight->SetUniforms(shader, reflection);
    spotCarA->SetUniforms(shader, reflection);
    spotCarB->SetUniforms(shader, reflection);
}

void Lights::ProcessCarMovement(glm::vec3 position, float angle)
{
    const glm::vec3 yVector = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec4 xVector = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
    glm::mat4 rot = glm::rotate(glm::mat4(1.0f), glm::radians(angle), yVector);
    xVector = rot * xVector;

    glm::mat4 left = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), yVector);
    glm::mat4 right = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), yVector);

    glm::vec3 leftVec = left * xVector;
    glm::vec3 rightVec = right * xVector;

    glm::vec3 spotAToMove = position + (0.033f * leftVec);
    glm::vec3 spotBToMove = position + (0.049f * rightVec);

    glm::vec3 targetVector = glm::vec3(xVector);
    glm::vec3 targetVectorA = targetVector + GetOffset(spotCarAOffset, targetVector);
    glm::vec3 targetVectorB = targetVector + GetOffset(spotCarBOffset, targetVector);

    spotCarA->SetPositionAndTarget(spotAToMove, targetVectorA, angle, glm::vec3(0.0f, 1.0f, 0.0f));
    spotCarB->SetPositionAndTarget(spotBToMove, targetVectorB, angle, glm::vec3(0.0f, 1.0f, 0.0f));
}

void Lights::ModifySpotDirection(uint dir)
{
    // 4 dirs
    int* tab = dir > 3 ? spotCarAOffset : spotCarBOffset;
    tab[dir % offset_size]++;
}

glm::vec3 Lights::GetOffset(int* offsetTab, glm::vec3 target)
{
    glm::mat4 rotation;
    glm::vec3 result;
    glm::vec4 rotated;
    glm::vec3 offset = glm::vec3(0.0f);
    const float scale = 0.05f;
    const glm::vec3 yVector = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec4 target4 = glm::vec4(target.x, target.y, target.z, 1.0f);

    for (int i = 0; i < offset_size; i++)
    {
        if (offsetTab[i] == 0) continue;
        switch(i) {
            case 0:
                rotation = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), yVector);
                rotated = rotation * target4;
                result = static_cast<float>(offsetTab[i]) * scale * glm::vec3(rotated);
                offset += result;
                break;
            case 1:
                rotation = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), yVector);
                rotated = rotation * target4;
                result = static_cast<float>(offsetTab[i]) * scale * glm::vec3(rotated);
                offset += result;
                break;
            case 2:
                offset += glm::vec3(0.0f, static_cast<float>(offsetTab[i]) * scale, 0.0f);
                break;
            case 3:
                offset += glm::vec3(0.0f, static_cast<float>(offsetTab[i]) * (- scale), 0.0f);
                break;
            default:
                //will not happen
                break;
        }
    }

    return offset;
}

void Lights::Draw(Shader &shader, bool reflection)
{
    pointLightA->Draw(shader, reflection);
    pointLightB->Draw(shader, reflection);
    spotLight->Draw(shader, reflection);
    spotCarA->Draw(shader, reflection);
    spotCarB->Draw(shader, reflection);
}

void Lights::SetNoon()
{
    dirLight->SetNoon();
}
void Lights::SetMidnight()
{
    dirLight->SetMidnight();
}
void Lights::ProcessFrame(float deltaTime)
{
    dirLight->NewFrame(deltaTime);
}

Lights::~Lights()
{
    delete pointLightA;
    delete pointLightB;
    delete spotLight;
    delete dirLight;
}

