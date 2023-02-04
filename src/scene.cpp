#include "scene.hpp"

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
    SetUpMeshTest();
}

void Scene::SetUpMeshTest()
{
    meshTest = loader.GenerateMeshTest();
    modelTest = new Model((*meshTest));

    modelTest->SetColor(glm::vec4(1.0f, 0.0f, 1.0f, 1.0f));
    modelTest->SetPlacement(glm::vec3(0.0f, 0.0f, 0.0f));
    modelTest->SetNoRotation();
    modelTest->SetScale(0.5f);
}

void Scene::Draw(Shader& shader)
{
    modelTest->Draw(shader);
}

Scene::~Scene()
{
    delete modelTest;
    delete meshTest;
}