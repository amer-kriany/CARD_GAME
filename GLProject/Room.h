
#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Polygon.cpp"

class Room
{
public:
    PolygonWithTexture floor;
    PolygonWithTexture backWall;
    PolygonWithTexture rightWall;
    PolygonWithTexture leftWall;
    PolygonWithTexture ceiling;
    PolygonWithTexture frontWall;
    PolygonWithTexture pic;

public:
    Room();

    void draw(Shader &shader);
};

#include "Room.h" // تمت إضافة علامات التنصيص

Room::Room() : floor(
                   {{-5.0f, -1.0f, -5.0f}, {5.0f, -1.0f, -5.0f}, {5.0f, -1.0f, 5.0f}, {-5.0f, -1.0f, 5.0f}},
                   {{0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}}),
               backWall(
                   {{-5.0f, -1.0f, -5.0f}, {5.0f, -1.0f, -5.0f}, {5.0f, 4.0f, -5.0f}, {-5.0f, 4.0f, -5.0f}},
                   {{0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}}),
               rightWall(
                   {{5.0f, -1.0f, -5.0f}, {5.0f, -1.0f, 5.0f}, {5.0f, 4.0f, 5.0f}, {5.0f, 4.0f, -5.0f}},
                   {{0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}}),
               leftWall(
                   {{-5.0f, -1.0f, -5.0f}, {-5.0f, -1.0f, 5.0f}, {-5.0f, 4.0f, 5.0f}, {-5.0f, 4.0f, -5.0f}},
                   {{0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}}),
               ceiling(
                   {{-5.0f, 4.0f, -5.0f}, {5.0f, 4.0f, -5.0f}, {5.0f, 4.0f, 5.0f}, {-5.0f, 4.0f, 5.0f}},
                   {{0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}}),
               frontWall(
                   {{-5.0f, -1.0f, 5.0f}, {5.0f, -1.0f, 5.0f}, {5.0f, 4.0f, 5.0f}, {-5.0f, 4.0f, 5.0f}},
                   {{0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}}),
               pic(
                   // 1. إحداثيات الرؤوس (تآكل عميق وواضح في الزاوية السفلية اليمنى فقط)
                   {
                       {-1.50f, 0.50f, 4.980f}, // الزاوية السفلية اليسرى (سليمة)
                       {0.30f, 0.50f, 4.980f},  // بداية التآكل من الحافة السفلية (واضح وعميق)
                       {0.36f, 0.57f, 4.976f},
                       {0.44f, 0.55f, 4.974f}, // نتوء متآكل
                       {0.50f, 0.65f, 4.970f},
                       {0.58f, 0.63f, 4.968f},
                       {0.63f, 0.75f, 4.964f}, // تعرج عميق نحو مركز الزاوية
                       {0.72f, 0.73f, 4.960f},
                       {0.76f, 0.86f, 4.956f}, // انثناء واضح للأمام بعيداً عن الحائط
                       {0.85f, 0.84f, 4.955f},
                       {0.88f, 0.96f, 4.958f}, // قمة التآكل باتجاه منتصف اللوحة
                       {0.98f, 0.94f, 4.960f},
                       {1.03f, 1.05f, 4.963f},
                       {1.12f, 1.04f, 4.965f},
                       {1.16f, 1.16f, 4.968f},
                       {1.25f, 1.15f, 4.970f},
                       {1.29f, 1.27f, 4.973f},
                       {1.38f, 1.26f, 4.975f},
                       {1.41f, 1.38f, 4.977f},
                       {1.47f, 1.40f, 4.978f},
                       {1.50f, 1.50f, 4.980f}, // نهاية التآكل على الحافة اليمنى
                       {1.50f, 2.50f, 4.980f}, // الزاوية العلوية اليمنى (سليمة)
                       {-1.50f, 2.50f, 4.980f} // الزاوية العلوية اليسرى (سليمة)
                   },
                   // 2. إحداثيات الصورة (UV) المطابقة تماماً لكل نقطة لمنع التمطيط
                   {
                       {0.000f, 0.000f},
                       {0.600f, 0.000f},
                       {0.620f, 0.035f},
                       {0.647f, 0.025f},
                       {0.667f, 0.075f},
                       {0.693f, 0.065f},
                       {0.710f, 0.125f},
                       {0.740f, 0.115f},
                       {0.753f, 0.180f},
                       {0.783f, 0.170f},
                       {0.793f, 0.230f},
                       {0.827f, 0.220f},
                       {0.843f, 0.275f},
                       {0.873f, 0.270f},
                       {0.887f, 0.330f},
                       {0.917f, 0.325f},
                       {0.930f, 0.385f},
                       {0.960f, 0.380f},
                       {0.970f, 0.440f},
                       {0.990f, 0.450f},
                       {1.000f, 0.500f},
                       {1.000f, 1.000f},
                       {0.000f, 1.000f}})

{
    // نقل مصفوفة الوحدة إلى الباني لتحسين الأداء
    glm::mat4 identity = glm::mat4(1.0f);
    floor.transformation(identity);
    backWall.transformation(identity);
    rightWall.transformation(identity);
    leftWall.transformation(identity);
    ceiling.transformation(identity);
    frontWall.transformation(identity);
    pic.transformation(identity);
}

void Room::draw(Shader &shader)
{

    floor.draw(shader);
    backWall.draw(shader);
    rightWall.draw(shader);
    leftWall.draw(shader);
    ceiling.draw(shader);
    frontWall.draw(shader);
    pic.draw(shader);
}
