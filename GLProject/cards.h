#pragma once

#include <vector>
#include <glm/glm.hpp>
#include "PolygonWithTexture.cpp"

class Cards
{
public:
    PolygonWithTexture card1;
    PolygonWithTexture card2;
    PolygonWithTexture card3;
    PolygonWithTexture card4;
    PolygonWithTexture card5;

    Cards()
        // الكرت الأول (الأسفل - فوق سطح الطاولة مباشرة عند y = 0.002)
        // الكرت الأول (الأسفل - فوق سطح الطاولة مباشرة عند y = 0.002)
        : card1({{-0.07f, 0.002f, -0.10f}, {0.07f, 0.002f, -0.10f}, {0.07f, 0.002f, 0.10f}, {-0.07f, 0.002f, 0.10f}}, // coordinates shape
                {{0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}}),                                            // coordinates texture(image)

          // الكرت الثاني (مرتفع قليلاً ومزاح لليمين وللأمام)
          card2({{-0.06f, 0.004f, -0.09f}, {0.08f, 0.004f, -0.09f}, {0.08f, 0.004f, 0.11f}, {-0.06f, 0.004f, 0.11f}},
                {{0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}}),

          // الكرت الثالث
          card3({{-0.05f, 0.006f, -0.08f}, {0.09f, 0.006f, -0.08f}, {0.09f, 0.006f, 0.12f}, {-0.05f, 0.006f, 0.12f}},
                {{0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}}),

          // الكرت الرابع
          card4({{-0.04f, 0.008f, -0.07f}, {0.10f, 0.008f, -0.07f}, {0.10f, 0.008f, 0.13f}, {-0.04f, 0.008f, 0.13f}},
                {{0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}}),

          // الكرت الخامس (الأعلى)
          card5({{-0.03f, 0.010f, -0.06f}, {0.11f, 0.010f, -0.06f}, {0.11f, 0.010f, 0.14f}, {-0.03f, 0.010f, 0.14f}},
                {{0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}})

    {
    }

    void draw(Shader &shader)
    {
        card1.draw(shader);
        card2.draw(shader);
        card3.draw(shader);
        card4.draw(shader);
        card5.draw(shader);
    }

    void transformation(glm::mat4 t)
    {
        card1.transformation(t);
        card2.transformation(t);
        card3.transformation(t);
        card4.transformation(t);
        card5.transformation(t);
    }

    void deleteBuffers() // for free RAM
    {
        card1.deleteBuffers();
        card2.deleteBuffers();
        card3.deleteBuffers();
        card4.deleteBuffers();
        card5.deleteBuffers();
    }
};

// in top main class:------------------
// #include "cards.h"
// _________________________________________________

// in main method : --------------------
//     Cards cards;

// unsigned int textureImage = loadTexture("./textures/Gambol.jpg"); // image for texture
// cards.card1.setTexture(textureImage);
// textureImage = loadTexture("./textures/Gambol.jpg"); // image for texture
// cards.card2.setTexture(textureImage);
// textureImage = loadTexture("./textures/Gambol.jpg");
// cards.card3.setTexture(textureImage);
// textureImage = loadTexture("./textures/Gambol.jpg");
// cards.card4.setTexture(textureImage);
// textureImage = loadTexture("./textures/Gambol.jpg");
// cards.card5.setTexture(textureImage);

// _________________________________________________

// in while loop -----------------------:
// cards.draw(ourShader);

// _________________________________________________
// :اذا صار خطأ بملف من ملفات البوليغون ضع التعليمة الاتية في اول الملف
// #pragma once
