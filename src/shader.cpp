#pragma once

#include "shader.hpp"

Shader::Shader(ShaderConfig &config)
{
    std::string verexString = readCode(config.vertexPath);
    std::string fragmentString = readCode(config.fragmentPath);
    const char* vertexShaderCode = verexString.c_str();
    const char* fragmentShaderCode = fragmentString.c_str();

    uint vertex, fragment;

    // vertex
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertexShaderCode, NULL);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX");

    // fragment
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragmentShaderCode, NULL);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");
    
    // program
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    checkCompileErrors(ID, "PROGRAM");

    // cleanup
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

std::string Shader::readCode(const std::string &pathToShader)
{
    std::ifstream fileStream;
    std::string code; 
    fileStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        fileStream.open(pathToShader);
        std::stringstream sStream;

        sStream << fileStream.rdbuf();

        fileStream.close();
        code = sStream.str();
    } catch(std::ifstream::failure& e) {
        std::cerr << "[ERROR] Shader code reading error (" << pathToShader << "): " << e.what() << std::endl;
    }

    return code.c_str();
}

void Shader::checkCompileErrors(unsigned int shader, std::string type)
{
    int success;
    char infoLog[logLength];
    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, logLength, NULL, infoLog);
            std::cerr << "[ERROR] Compilation error of type " << type << ": " << infoLog << std::endl;
        }
    } else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, logLength, NULL, infoLog);
            std::cerr << "[ERROR] Program linking error of type " << type << ": " << infoLog << std::endl;
        }
    }
}

int Shader::checkUnifromExistance(const std::string &name)
{
    int res = glGetUniformLocation(ID, name.c_str());
    if (res < 0) {
        std::cerr << "[ERROR] Uniform not found: " << name << std::endl;
    }
    return res;
}

void Shader::use()
{
    glUseProgram(ID);
}

void Shader::setUniform(const std::string &name, bool value)
{
    glUniform1i(checkUnifromExistance(name), (int)value); 
}

void Shader::setUniform(const std::string &name, int value)
{ 
    glUniform1i(checkUnifromExistance(name), value); 
}

void Shader::setUniform(const std::string &name, float value)
{ 
    glUniform1f(checkUnifromExistance(name), value); 
}

void Shader::setUniform(const std::string &name, glm::mat4 matrix)
{
    glUniformMatrix4fv(checkUnifromExistance(name), 1, GL_FALSE, glm::value_ptr(matrix));
}
void Shader::setUniform(const std::string &name, glm::vec3 vector)
{
    glUniform3fv(checkUnifromExistance(name), 1, glm::value_ptr(vector));
}