#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Polygon.cpp"

using namespace glm;

class RoomDecor {
private:
    std::vector<Polygon*> staticParts; 
    Polygon* clockPendulum;            
    vec3 clockPosition;

    
    void addBox(vec3 center, vec3 size, vec3 color) {
        float hx = size.x / 2.0f, hy = size.y / 2.0f, hz = size.z / 2.0f;
        vec3 p[8] = {
            center + vec3(-hx, -hy,  hz), center + vec3( hx, -hy,  hz),
            center + vec3( hx,  hy,  hz), center + vec3(-hx,  hy,  hz),
            center + vec3(-hx, -hy, -hz), center + vec3( hx, -hy, -hz),
            center + vec3( hx,  hy, -hz), center + vec3(-hx,  hy, -hz)
        };

        int faces[6][4] = {
            {0, 1, 2, 3}, {5, 4, 7, 6}, 
            {4, 0, 3, 7}, {1, 5, 6, 2}, 
            {3, 2, 6, 7}, {4, 5, 1, 0}  
        };

        for (int i = 0; i < 6; i++) {
            std::vector<vec3> v = { p[faces[i][0]], p[faces[i][1]], p[faces[i][2]], p[faces[i][3]] };
            
            float shade = 1.0f - (i * 0.08f);
            staticParts.push_back(new Polygon(v, color * shade));
        }
    }

public:
    RoomDecor() {
        
        // 1. القطعة الأولى: ساعة حائط بندولية (على الجدار الخلفي)
        
        clockPosition = vec3(-3.5f, 2.5f, -5.8f);
        // إطار الساعة الخشبي
        addBox(clockPosition, vec3(1.2f, 1.6f, 0.2f), vec3(0.35f, 0.18f, 0.08f));
        // قرص الساعة الأبيض
        addBox(clockPosition + vec3(0.0f, 0.35f, 0.11f), vec3(0.8f, 0.8f, 0.02f), vec3(0.95f, 0.92f, 0.85f));
        // عقارب الساعة
        addBox(clockPosition + vec3(0.0f, 0.45f, 0.13f), vec3(0.04f, 0.3f, 0.01f), vec3(0.1f, 0.1f, 0.1f));
        addBox(clockPosition + vec3(0.12f, 0.35f, 0.13f), vec3(0.25f, 0.04f, 0.01f), vec3(0.1f, 0.1f, 0.1f));

        // البندول الذهبي (مرسوم حول نقطة التعليق (0,0,0) ليتم تحريكه بسهولة)
        std::vector<vec3> pendVerts = {
            vec3(-0.03f,  0.0f, 0.0f),
            vec3( 0.03f,  0.0f, 0.0f),
            vec3( 0.03f, -0.7f, 0.0f),
            vec3(-0.03f, -0.7f, 0.0f)
        };
        clockPendulum = new Polygon(pendVerts, vec3(0.85f, 0.65f, 0.13f));

        // ==========================================
        // 2. القطعة الثانية: خزانة/مكتبة كتب جانبية (على الجدار الأيسر)
        // ==========================================
        vec3 shelfPos = vec3(-5.5f, 0.5f, 0.0f);
        // هيكل المكتبة
        addBox(shelfPos + vec3(0.0f, 1.5f, 0.0f), vec3(0.8f, 3.0f, 2.2f), vec3(0.42f, 0.24f, 0.12f));
        // الرفوف والكتب الملونة
        addBox(shelfPos + vec3(0.1f, 1.0f, -0.5f), vec3(0.5f, 0.45f, 0.12f), vec3(0.7f, 0.15f, 0.15f)); // كتاب أحمر
        addBox(shelfPos + vec3(0.1f, 1.0f, -0.35f), vec3(0.5f, 0.48f, 0.12f), vec3(0.15f, 0.35f, 0.65f)); // كتاب أزرق
        addBox(shelfPos + vec3(0.1f, 1.0f, -0.20f), vec3(0.5f, 0.42f, 0.12f), vec3(0.18f, 0.55f, 0.25f)); // كتاب أخضر

        // ==========================================
        // 3. القطعة الثالثة: مجسم كوكب زحل بحلقاته (ديكور معلق في الزاوية اليمنى)
        // ==========================================
        vec3 planetCenter = vec3(3.8f, 2.6f, -4.5f);
        // قلب الكوكب (شكل ثماني الأوجه/مكعب ماسي)
        addBox(planetCenter, vec3(0.75f, 0.75f, 0.75f), vec3(0.85f, 0.55f, 0.20f));
        // الحلقة الخارجية للكوكب
        addBox(planetCenter, vec3(1.8f, 0.04f, 1.8f), vec3(0.75f, 0.68f, 0.50f));
    }

    void draw(Shader& shader, float time) {
        // رسم الأجزاء الثابتة للقطع الثلاث
        for (Polygon* part : staticParts) {
            part->draw(shader);
        }

        // تحريك بندول الساعة ذهاباً وإياباً
        mat4 pendModel = mat4(1.0f);
        pendModel = translate(pendModel, clockPosition + vec3(0.0f, -0.05f, 0.12f));
        pendModel = rotate(pendModel, sin(time * 2.5f) * 0.35f, vec3(0.0f, 0.0f, 1.0f));
        clockPendulum->transformation(pendModel);
        clockPendulum->draw(shader);
    }
};