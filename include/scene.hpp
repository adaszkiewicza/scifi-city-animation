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
    Mesh* GenerateMeshFromFile(std::string path);
    std::pair<std::vector<ModelStatic*>, std::vector<Mesh*>> GenerateModelsComplex(std::string path);

private:
    std::string folderPath;
    uint LoadTexture(std::string path);
};

class Scene
{
public:
    Scene();
    void AdvanceMovement();
    void Draw(Shader &shader);
    void DrawLight(Shader &shader);

    ~Scene();
private:
    Material* testMaterial;
    Mesh* meshTest;
    ModelStatic* modelTestA;
    ModelStatic* modelTestB;

    Mesh* meshObj;
    ModelStatic* modelObj;

    Mesh* lightMesh;
    Lights* lights;

    ComplexStaticModel* room;
    std::vector<Mesh*> roomMeshes;

    ComplexStaticModel* car;
    std::vector<Mesh*> carMeshes;

    MeshLoader loader;

    void SetUpMeshesTest();
    void SetUpLights();
};