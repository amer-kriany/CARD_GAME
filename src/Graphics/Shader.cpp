#include "Shader.h"
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <iostream>
#include <sstream>

std::string Shader::readFile(const std::string& path) {
    std::ifstream file(path);
    if (!file) {
        std::cerr << "Cannot open shader file: " << path << "\n";
        return "";
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

unsigned int Shader::compile(unsigned int type, const std::string& source,
                             const std::string& label) {
    unsigned int shader = glCreateShader(type);
    const char* text = source.c_str();
    glShaderSource(shader, 1, &text, nullptr);
    glCompileShader(shader);

    int ok = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &ok);
    if (!ok) {
        char log[1024];
        glGetShaderInfoLog(shader, 1024, nullptr, log);
        std::cerr << label << " shader error:\n" << log << "\n";
    }
    return shader;
}

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath) {
    unsigned int vs = compile(GL_VERTEX_SHADER, readFile(vertexPath), "Vertex");
    unsigned int fs = compile(GL_FRAGMENT_SHADER, readFile(fragmentPath), "Fragment");

    program_ = glCreateProgram();
    glAttachShader(program_, vs);
    glAttachShader(program_, fs);
    glLinkProgram(program_);

    int ok = 0;
    glGetProgramiv(program_, GL_LINK_STATUS, &ok);
    if (!ok) {
        char log[1024];
        glGetProgramInfoLog(program_, 1024, nullptr, log);
        std::cerr << "Shader link error:\n" << log << "\n";
    }

    // Linked into the program, so the separate shader objects are not needed.
    glDeleteShader(vs);
    glDeleteShader(fs);
}

Shader::~Shader() {
    glDeleteProgram(program_);
}

void Shader::use() const {
    glUseProgram(program_);
}

void Shader::setMat4(const std::string& name, const glm::mat4& value) const {
    int location = glGetUniformLocation(program_, name.c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::setVec4(const std::string& name, const glm::vec4& value) const {
    int location = glGetUniformLocation(program_, name.c_str());
    glUniform4fv(location, 1, glm::value_ptr(value));
}

void Shader::setInt(const std::string& name, int value) const {
    int location = glGetUniformLocation(program_, name.c_str());
    glUniform1i(location, value);
}