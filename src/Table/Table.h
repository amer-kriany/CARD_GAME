
#pragma once

#include <array>
#include <string>

#include <glm/glm.hpp>

#include "Quad.h"
#include "Shader.h"
#include "Texture.h"

struct Zone {
    std::string name;
    glm::vec2 center;
    glm::vec2 size;
    glm::vec4 color;
};

class Table {
public:
    explicit Table(const std::string& woodTexturePath);

    void draw(const Shader& shader, const Quad& quad) const;

    const Zone& deck() const  { return zones_[0]; }
    const Zone& game() const  { return zones_[1]; }
    const Zone& enemy() const { return zones_[2]; }

    glm::vec2 size() const { return size_; }

private:
    Texture wood_;

    glm::vec2 size_{1.8f, 1.4f};

    // Order: deck, game, enemy
    std::array<Zone, 3> zones_;
};
