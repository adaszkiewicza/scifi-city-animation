#include "scene.hpp"
#include <iostream>

// required by tinyobjloader
#define TINYOBJLOADER_IMPLEMENTATION
// robust triangulation
// #define TINYOBJLOADER_USE_MAPBOX_EARCUT

#include "tiny_obj_loader.h"

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

Mesh* MeshLoader::GenerateMeshFromFile(std::string path)
{
    std::string relPath = folderPath + path;

    // using "newer" API
    tinyobj::ObjReaderConfig readerConfig;
    tinyobj::ObjReader reader;

    if (!reader.ParseFromFile(relPath, readerConfig)) {
        if (!reader.Error().empty())
            print_err(reader.Error());
        return NULL;
    }

    if (!reader.Warning().empty())
        print_err("- [WARN]:" + reader.Warning());

    auto& attributes = reader.GetAttrib();
    auto& shapes = reader.GetShapes();

    if (shapes.size() != 1) {
        print_err("Unexpected shape count");
        return NULL;
    }

    // not memory efficeint - works for me though

    std::vector<Vertex> verticies;
    std::vector<uint> indicies;

    size_t index_offset = 0;
    // faces loop
    for (size_t f = 0; f < shapes[0].mesh.num_face_vertices.size(); f++) 
    {
        size_t fv = size_t(shapes[0].mesh.num_face_vertices[f]); // assuming 3, but...

        // vert in face loop
        for (size_t v = 0; v < fv; v++)
        {
            tinyobj::index_t idx = shapes[0].mesh.indices[index_offset + v];
            
            float x = attributes.vertices[3 * size_t(idx.vertex_index) + 0];
            float y = attributes.vertices[3 * size_t(idx.vertex_index) + 1];
            float z = attributes.vertices[3 * size_t(idx.vertex_index) + 2];

            // assuming existance
            float nx = attributes.normals[3 * size_t(idx.normal_index) + 0];
            float ny = attributes.normals[3 * size_t(idx.normal_index) + 1];
            float nz = attributes.normals[3 * size_t(idx.normal_index) + 2];

            glm::vec3 coords = glm::vec3(x, y, z);
            glm::vec3 normal = glm::vec3(nx, ny, nz);

            verticies.push_back(Vertex(coords, normal));
        }
        std::cout << std::endl;
        index_offset += fv;
    }
    
    for (uint i = 0; i < verticies.size(); i++)
    {
        indicies.push_back(i);
    }

    return new Mesh(verticies, indicies);

}

Scene::Scene()
: loader("../objects/")
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
    modelTestA->SetPlacement(glm::vec3(10.0f, 0.0f, 0.0f));
    modelTestA->SetNoRotation();
    modelTestA->SetScale(0.1f);

    modelTestB->SetColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    modelTestB->SetPlacement(glm::vec3(0.0f, 1.0f, 0.0f));
    modelTestB->SetNoRotation();
    modelTestB->SetScale(0.1f);

    modelTestC->SetColor(glm::vec4(0.5f, 1.0f, 0.3f, 1.0f));
    modelTestC->SetPlacement(glm::vec3(1.0f, 0.0f, 0.0f));
    modelTestC->SetNoRotation();
    modelTestC->SetScale(0.1f);

    meshObj = loader.GenerateMeshFromFile("torus.obj");
    modelObj = new Model(*meshObj);
    modelObj->SetColor(glm::vec4(0.6f, 0.8f, 0.3f, 1.0f));
    modelObj->SetPlacement(glm::vec3(0.0f, 0.0f, 0.0f));
    modelObj->SetNoRotation();
    modelObj->SetScale(0.5f);

}

void Scene::Draw(Shader& shader)
{
    modelTestA->Draw(shader);
    modelTestB->Draw(shader);
    modelTestC->Draw(shader);
    modelObj->Draw(shader);
}

Scene::~Scene()
{
    delete modelTestA;
    delete modelTestB;
    delete modelTestC;
    delete meshTest;

    delete modelObj;
    delete meshObj;
}