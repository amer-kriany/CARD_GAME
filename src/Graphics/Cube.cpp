
#include "Cube.h"

#include <glad/glad.h>
#include <vector>

Cube::Cube()
{
    std::vector<float> vertices;

    auto addFace = [&](float normalX, float normalY, float normalZ,
                       const float corners[4][3])
    {
        const float uv[4][2] = {
            {0.0f, 0.0f},
            {1.0f, 0.0f},
            {1.0f, 1.0f},
            {0.0f, 1.0f}
        };

        const int indices[6] = {0, 1, 2, 0, 2, 3};

        for (int index : indices) {
            vertices.push_back(corners[index][0]);
            vertices.push_back(corners[index][1]);
            vertices.push_back(corners[index][2]);

            vertices.push_back(uv[index][0]);
            vertices.push_back(uv[index][1]);

            vertices.push_back(normalX);
            vertices.push_back(normalY);
            vertices.push_back(normalZ);
        }
    };

    const float front[4][3] = {
        {-0.5f, -0.5f,  0.5f},
        { 0.5f, -0.5f,  0.5f},
        { 0.5f,  0.5f,  0.5f},
        {-0.5f,  0.5f,  0.5f}
    };

    const float back[4][3] = {
        { 0.5f, -0.5f, -0.5f},
        {-0.5f, -0.5f, -0.5f},
        {-0.5f,  0.5f, -0.5f},
        { 0.5f,  0.5f, -0.5f}
    };

    const float right[4][3] = {
        {0.5f, -0.5f,  0.5f},
        {0.5f, -0.5f, -0.5f},
        {0.5f,  0.5f, -0.5f},
        {0.5f,  0.5f,  0.5f}
    };

    const float left[4][3] = {
        {-0.5f, -0.5f, -0.5f},
        {-0.5f, -0.5f,  0.5f},
        {-0.5f,  0.5f,  0.5f},
        {-0.5f,  0.5f, -0.5f}
    };

    const float top[4][3] = {
        {-0.5f, 0.5f,  0.5f},
        { 0.5f, 0.5f,  0.5f},
        { 0.5f, 0.5f, -0.5f},
        {-0.5f, 0.5f, -0.5f}
    };

    const float bottom[4][3] = {
        {-0.5f, -0.5f, -0.5f},
        { 0.5f, -0.5f, -0.5f},
        { 0.5f, -0.5f,  0.5f},
        {-0.5f, -0.5f,  0.5f}
    };

    addFace( 0.0f,  0.0f,  1.0f, front);
    addFace( 0.0f,  0.0f, -1.0f, back);
    addFace( 1.0f,  0.0f,  0.0f, right);
    addFace(-1.0f,  0.0f,  0.0f, left);
    addFace( 0.0f,  1.0f,  0.0f, top);
    addFace( 0.0f, -1.0f,  0.0f, bottom);

    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &vbo_);

    glBindVertexArray(vao_);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(
        GL_ARRAY_BUFFER,
        vertices.size() * sizeof(float),
        vertices.data(),
        GL_STATIC_DRAW
    );

    glVertexAttribPointer(
        0, 3, GL_FLOAT, GL_FALSE,
        8 * sizeof(float), (void*)0
    );
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(
        1, 2, GL_FLOAT, GL_FALSE,
        8 * sizeof(float),
        (void*)(3 * sizeof(float))
    );
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(
        2, 3, GL_FLOAT, GL_FALSE,
        8 * sizeof(float),
        (void*)(5 * sizeof(float))
    );
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

Cube::~Cube()
{
    glDeleteVertexArrays(1, &vao_);
    glDeleteBuffers(1, &vbo_);
}

void Cube::draw() const
{
    glBindVertexArray(vao_);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}
