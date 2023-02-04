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

class MeshLoader
{
public:
    MeshLoader(std::string folderPath);
    Mesh* GenerateMeshTest();

private:
    std::string folderPath; 
};

class Scene
{
public:
    Scene();
    void Draw(Shader &shader);

    ~Scene();
private:
    Mesh* meshTest;
    Model* modelTestA;
    Model* modelTestB;
    Model* modelTestC;

    MeshLoader loader;

    void SetUpMeshesTest();

};