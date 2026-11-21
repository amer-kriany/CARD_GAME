#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>
#include "PolygonWithTexture.cpp"

using namespace glm;

class CardAnimator {
public:
    PolygonWithTexture* cardMesh;
    vec3 basePosition;      // موقع البطاقة على الطاولة
    float currentAngle;     // الزاوية الحالية (0 = مسطحة، 90 = واقفة)
    float animSpeed;        // سرعة الحركة

    CardAnimator(PolygonWithTexture* mesh, vec3 pos, float speed = 220.0f) {
        cardMesh = mesh;
        basePosition = pos;
        currentAngle = 0.0f;
        animSpeed = speed;
    }

    // تحديث الحركة (Animation) في كل إطار
    void update(int cameraMode, float deltaTime) {
        // الهدف: 0 درجة إذا كانت الكاميرا 1 (مسطحة)، و 90 درجة إذا كانت الكاميرا 2 (واقفة)
        float targetAngle = (cameraMode == 2) ? 90.0f : 0.0f;

        if (currentAngle < targetAngle) {
            currentAngle = std::min(currentAngle + animSpeed * deltaTime, targetAngle);
        } else if (currentAngle > targetAngle) {
            currentAngle = std::max(currentAngle - animSpeed * deltaTime, targetAngle);
        }

        // تطبيق التحويلات الهندسية:
        // نفترض أن البطاقة مرسومة أفقياً على مستوى XZ ومركز دورانها عند حافتها السفلية
        mat4 model = mat4(1.0f);
        model = translate(model, basePosition);
        // رفع بسيط عن سطح الطاولة عند الوقوف حتى لا تتداخل مع الطاولة
        float liftY = (currentAngle / 90.0f) * 0.35f; 
        model = translate(model, vec3(0.0f, liftY, 0.0f));
        // التدوير حول محور X لتقف باتجاه اللاعب
        model = rotate(model, radians(currentAngle), vec3(1.0f, 0.0f, 0.0f));

        cardMesh->transformation(model);
    }

    void draw(Shader& shader) {
        cardMesh->draw(shader);
    }
};