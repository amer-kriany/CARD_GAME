#pragma once

#include <glm/glm.hpp>

#include "Graphics/PolygonWithTexture.h"
#include "Graphics/Shader.h"
#include "Graphics/Texture.h"


class Room {
public:
    Room();

    
    void draw(const Shader& shader, bool drawCeiling) const;

private:
   
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