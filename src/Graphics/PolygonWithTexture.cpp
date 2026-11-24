#include "PolygonWithTexture.h"

#include <stdexcept>

PolygonWithTexture::PolygonWithTexture(
    const std::vector<glm::vec3>& positions,
    const std::vector<glm::vec2>& texCoords,
    const glm::vec3& color
)
    : color_(color)
{
    if (positions.size() != texCoords.size() || positions.size() < 3)
    {
        throw std::invalid_argument(
            "PolygonWithTexture requires at least 3 matching vertices and UVs."
        );
    }

    
    for (std::size_t i = 0; i < positions.size(); ++i)
    {
        vertices_.push_back(positions[i].x);
        vertices_.push_back(positions[i].y);
        vertices_.push_back(positions[i].z);

        vertices_.push_back(texCoords[i].x);
        vertices_.push_back(texCoords[i].y);
    }

    glGenVertexArrays(1, &VAO_);
    glGenBuffers(1, &VBO_);

    glBindVertexArray(VAO_);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_);

    glBufferData(
        GL_ARRAY_BUFFER,
        vertices_.size() * sizeof(float),
        vertices_.data(),
        GL_STATIC_DRAW
    );

    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        5 * sizeof(float),
        (void*)0
    );

    glEnableVertexAttribArray(0);

   
    glVertexAttribPointer(
        1,
        2,
        GL_FLOAT,
        GL_FALSE,
        5 * sizeof(float),
        (void*)(3 * sizeof(float))
    );

    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

PolygonWithTexture::~PolygonWithTexture()
{
    glDeleteVertexArrays(1, &VAO_);
    glDeleteBuffers(1, &VBO_);
}

void PolygonWithTexture::draw(
    const Shader& shader,
    const Texture& texture,
    const glm::mat4& model,
    int material
) const
{
    shader.setMat4("model", model);
    shader.setInt("useTexture", material);
    shader.setInt("ourTexture", 0);

    shader.setVec4(
        "tint",
        glm::vec4(color_, 1.0f)
    );

    texture.bind();

    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO_);

    glDrawArrays(
        GL_TRIANGLE_FAN,
        0,
        static_cast<GLsizei>(vertices_.size() / 5)
    );

    glBindVertexArray(0);
}