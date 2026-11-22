#pragma once

#include <string>
#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "Shader.h"

// A character (or any model) loaded from a .glb file.
//
// The model is rigged (it has a skeleton), but we do not animate it live.
// At load time we freeze ONE moment of ONE animation (for example frame 0
// of "Idle") and bake that pose into the vertices. After that it is just
// a normal static mesh, which keeps the code small.
class Model {
public:
    // animationName: e.g. "CharacterArmature|Idle". Empty = rest pose.
    Model(
        const std::string& path,
        const std::string& animationName,
        float animationTime
    );

    ~Model();

    Model(const Model&) = delete;
    Model& operator=(const Model&) = delete;

    // The model's origin is the middle of its feet, so "placement" puts
    // the feet where you want them (translate / rotate / scale).
    void draw(const Shader& shader, const glm::mat4& placement) const;

    // Height of the model in its own units (before scaling).
    float height() const { return height_; }

private:
    // One colored piece (the glb splits the character by material).
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