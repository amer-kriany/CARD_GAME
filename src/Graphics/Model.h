#pragma once

#include <string>
#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "Shader.h"


class Model {
public:
    Model(
        const std::string& path,
        const std::string& animationName,
        float animationTime
    );

    ~Model();

    Model(const Model&) = delete;
    Model& operator=(const Model&) = delete;

    void draw(const Shader& shader, const glm::mat4& placement) const;

    float height() const { return height_; }

private:
    struct Part {
        GLuint vao = 0;
        GLuint vbo = 0;
        GLuint ebo = 0;
        GLsizei indexCount = 0;
        glm::vec3 color{1.0f};
    };

    std::vector<Part> parts_;
    float height_ = 0.0f;
};