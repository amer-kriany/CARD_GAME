
#pragma once

#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "Shader.h"
#include "Texture.h"

class PolygonWithTexture
{
public:
    PolygonWithTexture(
        const std::vector<glm::vec3>& positions,
        const std::vector<glm::vec2>& texCoords,
        const glm::vec3& color = glm::vec3(1.0f)
    );

    ~PolygonWithTexture();

    PolygonWithTexture(const PolygonWithTexture&) = delete;
    PolygonWithTexture& operator=(const PolygonWithTexture&) = delete;

    void draw(
        const Shader& shader,
        const Texture& texture,
        const glm::mat4& model
    ) const;

private:
    std::vector<float> vertices_;

    glm::vec3 color_;

    GLuint VAO_ = 0;
    GLuint VBO_ = 0;
};
