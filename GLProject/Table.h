
#pragma once

#include "Polygon.cpp"
#include <vector>
#include <glm/glm.hpp>
#include "PolygonWithTexture.cpp"

class Table
{
public:
    PolygonWithTexture top;
    Polygon leg1;
    Polygon leg2;
    Polygon leg3;
    Polygon leg4;

    Table()
        : top({{-1.0f, 0.0f, -0.6f}, {1.0f, 0.0f, -0.6f}, {1.0f, 0.0f, 0.6f}, {-1.0f, 0.0f, 0.6f}}, // coordinates shape
              {{0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}}),                            // coordinates texture(image)

          leg1(
              {{-0.9f, -1.0f, 0.5f},
               {-0.75f, -1.0f, 0.5f},
               {-0.75f, 0.0f, 0.5f},
               {-0.9f, 0.0f, 0.5f}},
              {0.25f, 0.12f, 0.04f}),

          leg2(
              {{0.75f, -1.0f, 0.5f},
               {0.9f, -1.0f, 0.5f},
               {0.9f, 0.0f, 0.5f},
               {0.75f, 0.0f, 0.5f}},
              {0.25f, 0.12f, 0.04f}),

          leg3(
              {{-0.9f, -1.0f, -0.5f},
               {-0.75f, -1.0f, -0.5f},
               {-0.75f, 0.0f, -0.5f},
               {-0.9f, 0.0f, -0.5f}},
              {0.25f, 0.12f, 0.04f}),

          leg4(
              {{0.75f, -1.0f, -0.5f},
               {0.9f, -1.0f, -0.5f},
               {0.9f, 0.0f, -0.5f},
               {0.75f, 0.0f, -0.5f}},
              {0.25f, 0.12f, 0.04f})
    {
    }

    void draw(Shader &shader)
    {
        top.draw(shader);

        leg1.draw(shader);
        leg2.draw(shader);
        leg3.draw(shader);
        leg4.draw(shader);
    }

    void transformation(glm::mat4 t)
    {
        top.transformation(t);

        leg1.transformation(t);
        leg2.transformation(t);
        leg3.transformation(t);
        leg4.transformation(t);
    }

    void deleteBuffers()
    {
        top.deleteBuffers();

        leg1.deleteBuffers();
        leg2.deleteBuffers();
        leg3.deleteBuffers();
        leg4.deleteBuffers();
    }
};
