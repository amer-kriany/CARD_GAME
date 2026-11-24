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

    mat4 getViewMatrix() {
        if (currentMode == 1) {
            vec3 pos = vec3(0.0f, 5.5f, 0.01f); 
            vec3 target = vec3(0.0f, 0.0f, 0.0f);
            vec3 up = vec3(0.0f, 0.0f, -1.0f);
            return lookAt(pos, target, up);
        } else {
           
            vec3 pos = vec3(0.0f, 1.9f, 4.6f);
            vec3 target = vec3(0.0f, -0.2f, 0.0f);
            vec3 up = vec3(0.0f, 1.0f, 0.0f);
            return lookAt(pos, target, up);
        }
    }
};