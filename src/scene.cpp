#include "scene.hpp"
#include <iostream>
#include <map>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// required by tinyobjloader
#define TINYOBJLOADER_IMPLEMENTATION
// robust triangulation
#define TINYOBJLOADER_USE_MAPBOX_EARCUT

#include "tiny_obj_loader.h"

MeshLoader::MeshLoader(std::string folderPath)
: folderPath(folderPath) 
{
    //stbi_set_flip_vertically_on_load(true);
}

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

uint MeshLoader::LoadTexture(std::string path)
{
    uint texture;

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glEnable(GL_TEXTURE_2D) ;
     // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;

    unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cerr << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    return texture;
}

static Mesh* MeshSpawner(tinyobj::attrib_t attributes, std::vector<tinyobj::shape_t> shapes, size_t offset)
{
    // not memory efficeint - works for me though

    std::vector<Vertex> verticies;
    std::vector<uint> indicies;

    //auto shapes = *shapes_ptr;

    size_t shape_id = offset;
    size_t index_offset = 0;
    // faces loop
    for (size_t f = 0; f < shapes[shape_id].mesh.num_face_vertices.size(); f++) 
    {
        size_t fv = size_t(shapes[shape_id].mesh.num_face_vertices[f]); // assuming 3, but...

        // vert in face loop
        for (size_t v = 0; v < fv; v++)
        {
            tinyobj::index_t idx = shapes[shape_id].mesh.indices[index_offset + v];
            
            float x = attributes.vertices[3 * size_t(idx.vertex_index) + 0];
            float y = attributes.vertices[3 * size_t(idx.vertex_index) + 1];
            float z = attributes.vertices[3 * size_t(idx.vertex_index) + 2];

            // assuming existance
            float nx = attributes.normals[3 * size_t(idx.normal_index) + 0];
            float ny = attributes.normals[3 * size_t(idx.normal_index) + 1];
            float nz = attributes.normals[3 * size_t(idx.normal_index) + 2];

            float tx = 0.0f, ty = 0.0f;
            if (idx.texcoord_index >= 0) {
                tx = attributes.texcoords[2*size_t(idx.texcoord_index)+0];
                ty = attributes.texcoords[2*size_t(idx.texcoord_index)+1];
            }

            glm::vec3 coords = glm::vec3(x, y, z);
            glm::vec3 normal = glm::vec3(nx, ny, nz);
            glm::vec2 texture = glm::vec2(tx, ty);

            verticies.push_back(Vertex(coords, normal, texture));
        }
        index_offset += fv;
    }
    
    for (uint i = 0; i < verticies.size(); i++)
    {
        indicies.push_back(i);
    }

    return new Mesh(verticies, indicies);
}

std::pair<std::vector<ModelStatic*>, std::vector<Mesh*>> MeshLoader::GenerateModelsComplex(std::string path)
{
    std::string relPath = folderPath + path;

    // using "newer" API
    tinyobj::ObjReaderConfig readerConfig;
    tinyobj::ObjReader reader;

    if (!reader.ParseFromFile(relPath, readerConfig)) {
        if (!reader.Error().empty())
            print_err(reader.Error());
    }

    if (!reader.Warning().empty())
        print_err("- [WARN]:" + reader.Warning());

    auto& shapes = reader.GetShapes();
    auto& materials = reader.GetMaterials();
    auto& attributes = reader.GetAttrib();

    std::vector<ModelStatic*> models;
    std::vector<Mesh*> meshes;
    for (size_t i = 0; i < shapes.size(); i++)
    {
        // assume entire obj has same material
        auto material_id = shapes[i].mesh.material_ids[0];
        auto shape_material = materials[material_id];

        Mesh* mesh = MeshSpawner(attributes, shapes, i);

        glm::vec3 diffuse = glm::vec3(shape_material.diffuse[0], shape_material.diffuse[1], shape_material.diffuse[2]);
        glm::vec3 specular = glm::vec3(shape_material.specular[0], shape_material.specular[1], shape_material.specular[2]);
        glm::vec3 ambient = glm::vec3(shape_material.ambient[0], shape_material.ambient[1], shape_material.ambient[2]);

        Material material = Material(diffuse, specular, shape_material.shininess, ambient);

        ModelStatic* model = new ModelStatic(*mesh, material);
        models.push_back(model);
        meshes.push_back(mesh);
    }

    return std::pair{models, meshes};
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
    auto& materials = reader.GetMaterials();

    if (shapes.size() != 1) {
        print_err("Unexpected shape count");
        return NULL;
    }

    return MeshSpawner(attributes, shapes, 0);

}

Scene::Scene()
: loader("../objects/")
{
    SetUpMeshesTest();
    SetUpLights();
}

void Scene::SetUpMeshesTest()
{
    testMaterial = new Material(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.5f), 32.0f);
    meshTest = loader.GenerateMeshTest();
    modelTestA = new ModelStatic(*meshTest, *testMaterial);
    modelTestB = new ModelStatic(*meshTest, *testMaterial);

    modelTestA->SetColor(glm::vec3(1.0f, 0.0f, 1.0f));
    modelTestA->SetPlacement(glm::vec3(10.0f, 0.0f, 0.0f));
    modelTestA->SetNoRotation();
    modelTestA->SetScale(0.1f);

    modelTestB->SetColor(glm::vec3(1.0f, 0.0f, 0.0f));
    modelTestB->SetPlacement(glm::vec3(-0.5f, -0.5f, 0.0f));
    modelTestB->SetNoRotation();
    modelTestB->SetScale(1.0f);

    meshObj = loader.GenerateMeshFromFile("torus.obj");
    modelObj = new ModelStatic(*meshObj, *testMaterial);
    modelObj->SetColor(glm::vec3(1.0f, 1.0f, 1.0f));
    modelObj->SetPlacement(glm::vec3(0.0f, 0.0f, 0.0f));
    modelObj->SetNoRotation();
    modelObj->SetScale(0.5f);

    auto loaded = loader.GenerateModelsComplex("another_bedroom.obj");
    std::vector<ModelStatic*> models = loaded.first;
    //std::vector<ModelStatic*> dupa;
    //dupa.push_back(models[0]);
    roomMeshes = loaded.second;
    room = new ComplexStaticModel(models);
    room->SetColor(glm::vec3(1.0f, 1.0f, 1.0f));
    room->SetPlacement(glm::vec3(0.0f, -2.0f, 1.0f));
    room->SetRotation(180.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    //city->SetNoRotation();
    room->SetScale(0.4f);
    //city->SetScale(1.0f);*/

    loaded = loader.GenerateModelsComplex("Jeep_Renegade_2016.obj");
    models = loaded.first;
    //std::vector<ModelStatic*> dupa;
    //dupa.push_back(models[0]);
    carMeshes = loaded.second;
    car = new ComplexStaticModel(models);
    car->SetColor(glm::vec3(1.0f, 1.0f, 1.0f));
    car->SetPlacement(glm::vec3(1.0f, -5.0f, 0.0f));
    //lamp->SetRotation(180.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    car->SetNoRotation();
    car->SetScale(0.1f);
    //city->SetScale(1.0f);*/

}

void Scene::SetUpLights()
{
    lightMesh = loader.GenerateMeshTest();
    lights = new Lights(*lightMesh);
}

void Scene::AdvanceMovement()
{
    // pass for now
}

void Scene::Draw(Shader& shader)
{
    lights->SetUniforms(shader);

    //modelTestA->Draw(shader);
    //modelTestB->Draw(shader);
    //modelObj->Draw(shader);

    room->Draw(shader);
    car->Draw(shader);
}

void Scene::DrawLight(Shader &shader)
{
    lights->Draw(shader);
}

Scene::~Scene()
{
    delete modelTestA;
    delete modelTestB;
    delete meshTest;

    delete modelObj;
    delete meshObj;

    delete lights;
    delete lightMesh;

    for (size_t i = 0; i < roomMeshes.size(); i++)
        delete roomMeshes[i];
    delete room;

    for (size_t i = 0; i < carMeshes.size(); i++)
        delete carMeshes[i];
    delete car;
}