#include "scene.hpp"
#include <iostream>
MeshLoader::MeshLoader(std::string folderPath)
: folderPath(folderPath) {}

Mesh* MeshLoader::GenerateMeshTest()
{
    // static object for now :)
    std::vector<glm::vec3> verts = {
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 1.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 1.0f),
        glm::vec3(1.0f, 0.0f, 0.0f),
        glm::vec3(1.0f, 0.0f, 1.0f),
        glm::vec3(1.0f, 1.0f, 0.0f),
        glm::vec3(1.0f, 1.0f, 1.0f),
    };

    std::vector<glm::vec3> normals = {
        glm::vec3(0.0f, 0.0f, 1.0f),
        glm::vec3(0.0f, 0.0f, -1.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        glm::vec3(0.0f, -1.0f, 0.0f),
        glm::vec3(1.0f, 0.0f, 0.0f),
        glm::vec3(-1.0f, 0.0f, 0.0f),
    };

    std::vector<Vertex> entities = {
        Vertex(verts[0], normals[1]),
        Vertex(verts[6], normals[1]),
        Vertex(verts[4], normals[1]),
        Vertex(verts[1], normals[1]),

        Vertex(verts[0], normals[5]),
        Vertex(verts[3], normals[5]),
        Vertex(verts[2], normals[5]),
        Vertex(verts[1], normals[5]),

        Vertex(verts[2], normals[2]),
        Vertex(verts[7], normals[2]),
        Vertex(verts[6], normals[2]),
        Vertex(verts[3], normals[2]),

        Vertex(verts[4], normals[4]),
        Vertex(verts[6], normals[4]),
        Vertex(verts[7], normals[4]),
        Vertex(verts[5], normals[4]),

        Vertex(verts[0], normals[3]),
        Vertex(verts[4], normals[3]),
        Vertex(verts[5], normals[3]),
        Vertex(verts[1], normals[3]),

        Vertex(verts[1], normals[0]),
        Vertex(verts[5], normals[0]),
        Vertex(verts[7], normals[0]),
        Vertex(verts[3], normals[0]),
    };

    std::vector<uint> indicies = {
        0, 1, 2,
        0, 3, 1,

        4, 5, 6,
        4, 7, 5,

        8, 9, 10,
        8, 11, 9,

        12, 13, 14,
        12, 14, 15,

        16, 17, 18,
        16, 18, 19,

        20, 21, 22,
        20, 22, 23
    };

    return new Mesh(entities, indicies);
}

Scene::Scene()
: loader("lol")
{
    SetUpMeshesTest();
}

void Scene::SetUpMeshesTest()
{
    meshTest = loader.GenerateMeshTest();
    modelTestA = new Model((*meshTest));
    modelTestB = new Model((*meshTest));
    modelTestC = new Model((*meshTest));

    modelTestA->SetColor(glm::vec4(1.0f, 0.0f, 1.0f, 1.0f));
    modelTestA->SetPlacement(glm::vec3(0.0f, 0.0f, 0.0f));
    modelTestA->SetNoRotation();
    modelTestA->SetScale(0.5f);

    modelTestB->SetColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    modelTestB->SetPlacement(glm::vec3(0.0f, 0.3f, -1.0f));
    modelTestB->SetRotation(45.0f, glm::vec3(0.0f, 0.0f, 1.0f));
    modelTestB->SetScale(0.5f);

    modelTestC->SetColor(glm::vec4(0.5f, 1.0f, 0.3f, 1.0f));
    modelTestC->SetPlacement(glm::vec3(0.0f, 0.5f, 0.5f));
    modelTestC->SetNoRotation();
    modelTestC->SetScale(0.5f);
}

void Scene::Draw(Shader& shader)
{
    modelTestA->Draw(shader);
    modelTestB->Draw(shader);
    modelTestC->Draw(shader);
}

Scene::~Scene()
{
    delete modelTestA;
    delete modelTestB;
    delete modelTestC;
    delete meshTest;
}