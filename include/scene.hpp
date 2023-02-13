#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <string>

#include "common.hpp"
#include "shader.hpp"
#include "mesh.hpp"
#include "model.hpp"
#include "lights.hpp"

class MeshLoader
{
public:
    MeshLoader(std::string folderPath);
    Mesh* GenerateMeshTest();
    Mesh* GenerateMeshMirror();
    Mesh* GenerateMeshFromFile(std::string path);
    std::pair<std::vector<ModelStatic*>, std::vector<Mesh*>> GenerateModelsComplex(std::string path);

private:
    std::string folderPath;
    int LoadTexture(std::string path, uint* tex);
};

class Scene
{
public:
    Scene();
    void AdvanceMovement(float deltaTime);
    void Draw(Shader &shader, bool reflection=false);
    void DrawLight(Shader &shader, bool reflection=false);
    void SwitchAnimation();
    void SetNoon();
    void SetMidnight();
    void ProcessFrame(float deltaTime);
    void DrawMirror(Shader &shader);
    void SwitchVibrations();
    void ModifyCarSpotDirection(uint dir);

    std::pair<glm::vec3, float> GetCarPositionAndTarget();

    ~Scene();
private:
    Material* testMaterial;
    Mesh* meshTest;

    Mesh* lightMesh;
    Lights* lights;

    Mesh* mirrorMesh;
    Model* mirror;

    ComplexStaticModel* room;
    std::vector<Mesh*> roomMeshes;

    ComplexStaticModel* car;
    std::vector<Mesh*> carMeshes;

    MeshLoader loader;

    bool animationOn = false;
    uint animationPhase = 0;
    const uint totalPhases = 4;
    bool rotating = false;
    float totalAngle = 0;
    glm::vec3 carTarget;
    glm::vec3 carPosition;
    glm::vec3 tempCarPosition;

    const float maxZ = 1.0f;
    const float maxX = 2.3f;
    const float minZ = -2.3f;
    const float minX = -2.4f;

    bool vibrationsOn = false;

    void SetUpMeshesTest();
    void SetUpLights();
    void Animation(float deltaTime);
    glm::vec3 GetVibrationVector();
};