#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

#include "common.hpp"

class ShaderConfig
{
private:
    const std::string folderPath = "../shaders/";

    const std::string vertexBasePath = "shader.vs";
    const std::string fragmentBasePath = "shader.fs";
public:
    const std::string vertexPath = folderPath + vertexBasePath;
    const std::string fragmentPath = folderPath + fragmentBasePath;

    ShaderConfig();
};

class Shader
{
private:
    const int logLength = 1024;

    std::string readCode(const std::string &pathToShader);
    void checkCompileErrors(unsigned int shader, std::string type);
    int checkUnifromExistance(const std::string &name);
public:
    uint ID;
  
    Shader(ShaderConfig &config);
    void use();

    void setUniform(const std::string &name, bool value);
    void setUniform(const std::string &name, int value);   
    void setUniform(const std::string &name, float value);
    void setUniform(const std::string &name, glm::mat4 matrix);
};