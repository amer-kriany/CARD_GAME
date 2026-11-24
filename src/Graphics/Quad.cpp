
#include "Quad.h"
#include <glad/glad.h>

Quad::Quad()
{
    float vertices[] = {
        // Position              // UV
        -0.5f, -0.5f, 0.0f,       0.0f, 0.0f,
         0.5f, -0.5f, 0.0f,       1.0f, 0.0f,
        -0.5f,  0.5f, 0.0f,       0.0f, 1.0f,
         0.5f,  0.5f, 0.0f,       1.0f, 1.0f
    };

    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &vbo_);

    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);

    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(vertices),
        vertices,
        GL_STATIC_DRAW
    );

    glVertexAttribPointer(
        0, 3, GL_FLOAT, GL_FALSE,
        5 * sizeof(float), (void*)0
    );
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(
        1, 2, GL_FLOAT, GL_FALSE,
        5 * sizeof(float),
        (void*)(3 * sizeof(float))
    );
    glEnableVertexAttribArray(1);

    glDisableVertexAttribArray(2);
    glVertexAttrib3f(2, 0.0f, 0.0f, 1.0f);

    glBindVertexArray(0);
}

Quad::~Quad()
{
    glDeleteVertexArrays(1, &vao_);
    glDeleteBuffers(1, &vbo_);
}

void Quad::draw() const
{
    glBindVertexArray(vao_);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}
