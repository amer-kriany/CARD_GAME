#pragma once
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

class DualCamera {
public:
    int currentMode; // 1 = Top-Down (فوق الطاولة), 2 = Player View (منظور اللاعب)

    DualCamera() {
        currentMode = 1; // الحالة الافتراضية عند التشغيل
    }

    // معالجة أزرار التبديل 1 و 2
    void processKeyboard(GLFWwindow* window) {
        if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
            currentMode = 1;
        }
        if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
            currentMode = 2;
        }
    }

    // إرجاع مصفوفة الرؤية (View Matrix) حسب الحالة النشطة
    mat4 getViewMatrix() {
        if (currentMode == 1) {
            // الحالة 1: الكاميرا من فوق الطاولة تماماً تنظر للأسفل
            vec3 pos = vec3(0.0f, 5.5f, 0.01f); // إزاحة بسيطة جداً في Z لتجنب تطابق متجه الأعلى
            vec3 target = vec3(0.0f, 0.0f, 0.0f);
            vec3 up = vec3(0.0f, 0.0f, -1.0f);
            return lookAt(pos, target, up);
        } else {
            // الحالة 2: الكاميرا من منظور اللاعب (جهة اللاعب من الطاولة)
            vec3 pos = vec3(0.0f, 2.2f, 4.2f);
            vec3 target = vec3(0.0f, 0.4f, 0.0f);
            vec3 up = vec3(0.0f, 1.0f, 0.0f);
            return lookAt(pos, target, up);
        }
    }
};