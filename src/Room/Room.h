#pragma once

#include <glm/glm.hpp>

#include "Graphics/PolygonWithTexture.h"
#include "Graphics/Shader.h"
#include "Graphics/Texture.h"

// Waseem's room (floor, walls, ceiling, torn poster) ported to our classes.
// Everything is in WORLD space, so draw it with the "world" uniform = identity.
class Room {
public:
    Room();

    // The ceiling is optional: camera 1 looks down from ABOVE the ceiling,
    // so drawing it there would hide the whole table.
    void draw(const Shader& shader, bool drawCeiling) const;

private:
    // Textures first: the polygons below don't own them, they are
    // only handed to draw().
    Texture floorTexture_;
    Texture wallTexture_;
    Texture posterTexture_;

    PolygonWithTexture floor_;
    PolygonWithTexture backWall_;
    PolygonWithTexture leftWall_;
    PolygonWithTexture rightWall_;
    PolygonWithTexture frontWall_;
    PolygonWithTexture ceiling_;
    PolygonWithTexture poster_;
};