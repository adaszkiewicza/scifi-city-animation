#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <string>

#include "common.hpp"
#include "shader.hpp"

struct Vertex {
    Vertex(glm::vec3 pos, glm::vec3 norm)
    : Position(pos), Normal(norm) {};
    
    glm::vec3 Position;
    glm::vec3 Normal;
};

class Mesh {
    public:
        // mesh data
        std::vector<Vertex> vertices;
        std::vector<uint> indices;

        Mesh(std::vector<Vertex> vertices, std::vector<uint> indices);
        void Draw(Shader &shader);
    private:
        //  render data
        unsigned int VAO, VBO, EBO;

        void setupMesh();
}; 
