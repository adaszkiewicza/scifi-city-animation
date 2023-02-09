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

private:
    std::string folderPath; 
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

    MeshLoader loader;

    void SetUpMeshesTest();
    void SetUpLights();
};