#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Cards/Cards.h"
#include "Graphics/Texture.h"
#include "Graphics/Model.h"
#include "Room/Room.h"
#include "Room/RoomDecor.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <algorithm>

#include "Graphics/Quad.h"
#include "Graphics/Shader.h"
#include "Table/Table.h"
#include "Table/TableLegs.h"
#include "Cards/CardManager.h"
#include "Cards/CardAnimator.h"
#include "Camera/DualCamera.h"
#include "WorldConfig.h"

#ifndef ASSET_DIR
#define ASSET_DIR "./"
#endif

static void onResize(GLFWwindow*, int width, int height)
{
    glViewport(0, 0, width, height);
}

int main()
{
    if (!glfwInit())
    {
        std::cerr << "GLFW initialization failed\n";
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(
        800,
        600,
        "Royal Card Game",
        nullptr,
        nullptr
    );

    if (window == nullptr)
    {
        std::cerr << "Window creation failed\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, onResize);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "GLAD initialization failed\n";
        glfwTerminate();
        return -1;
    }

    glViewport(0, 0, 800, 600);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // We are in real 3D now, so we need a depth buffer.
    glEnable(GL_DEPTH_TEST);

    {
        Shader shader(
            ASSET_DIR "shaders/basic.vert",
            ASSET_DIR "shaders/basic.frag"
        );

        Quad quad;

        Table table(
            ASSET_DIR "textures/wood.png"
        );

        Texture cardTexture(
            ASSET_DIR "textures/wood.png"
        );

        // Wood for the legs. Kept separate from cardTexture because the
        // cards will get their own artwork later.
        Texture woodTexture(
            ASSET_DIR "textures/wood.png"
        );

        TableLegs tableLegs;

        Cards cards;

        CardManager cardManager;
        CardAnimator cardAnimator;

        // The room, decorations and character must be constructed while the
        // OpenGL context is alive, just like the table and cards.
        Room room;
        RoomDecor roomDecor;

        // The GLB is included in the project at models/Adventurer.glb.
        // Freeze its Idle animation at 0.8 seconds for a stable standing pose.
        Model adventurer(
            ASSET_DIR "models/Adventurer.glb",
            "CharacterArmature|Idle",
            0.8f
        );

        // Fadi's camera: keys 1 and 2 switch between the two fixed views.
        DualCamera camera;

        bool gWasPressed = false;

        float lastFrame = 0.0f;

        while (!glfwWindowShouldClose(window))
        {
            float currentFrame = static_cast<float>(glfwGetTime());

            float deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;

            // Avoid huge animation jumps if the window freezes briefly.
            deltaTime = std::min(deltaTime, 0.05f);

            if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            {
                glfwSetWindowShouldClose(window, true);
            }

            // Keys 1 / 2 -> camera mode.
            camera.processKeyboard(window);

            // Detect a new G key press.
            bool gIsPressed =
                glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS;

            if (gIsPressed && !gWasPressed)
            {
                // Start an animation only if a card was successfully moved.
                if (cardManager.playRandomCard())
                {
                    const Card& selectedCard =
                        cardManager.playedCards().back();

                    int targetIndex =
                        cardManager.playedCount() - 1;

                    cardAnimator.start(
                        selectedCard.id(),
                        targetIndex
                    );
                }
            }

            gWasPressed = gIsPressed;

            cardAnimator.update(deltaTime, camera.currentMode);

            glClearColor(
                0.025f,
                0.018f,
                0.015f,
                1.0f
            );

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // Perspective projection that follows the window shape.
            int framebufferWidth = 0;
            int framebufferHeight = 0;
            glfwGetFramebufferSize(
                window,
                &framebufferWidth,
                &framebufferHeight
            );

            float aspect =
                static_cast<float>(framebufferWidth) /
                static_cast<float>(std::max(framebufferHeight, 1));

            glm::mat4 projection = glm::perspective(
                glm::radians(45.0f),
                aspect,
                0.1f,
                100.0f
            );

            shader.use();
            shader.setMat4("projection", projection);
            shader.setMat4("view", camera.getViewMatrix());

            // ---- Room ----
            // Room geometry is already authored in world coordinates.
            shader.setMat4("world", glm::mat4(1.0f));
            room.draw(shader, camera.currentMode == 2);

            // ---- Room Decorations ----
            roomDecor.draw(shader, currentFrame);

            // ---- Adventurer character ----
            // Put the character in the back-right corner, standing on the
            // room floor and facing into the room.
            glm::mat4 characterPlacement(1.0f);
            characterPlacement = glm::translate(
                characterPlacement,
                glm::vec3(3.45f, world::FLOOR_Y, -3.25f)
            );
            characterPlacement = glm::rotate(
                characterPlacement,
                glm::radians(180.0f),
                glm::vec3(0.0f, 1.0f, 0.0f)
            );
            characterPlacement = glm::scale(
                characterPlacement,
                glm::vec3(1.25f)
            );
            shader.setMat4("world", glm::mat4(1.0f));
            adventurer.draw(shader, characterPlacement);

            // ---- Legs ----
            // Drawn BEFORE the table, with depth on. Legs are already in
            // world space, so "world" is identity.
            shader.setMat4("world", glm::mat4(1.0f));
            tableLegs.draw(shader, woodTexture);

            // ---- Table ----
            // Table.cpp still draws in its old XY layout and sets "model"
            // itself. The "world" matrix lays that whole thing flat on the
            // XZ floor plane (and scales it up) without touching Table.cpp.
            // The table is painted in layers, so it skips the depth test
            // and is simply drawn first.
            shader.setMat4("world", world::tableToWorld());

            glDisable(GL_DEPTH_TEST);
            table.draw(shader, quad);
            glEnable(GL_DEPTH_TEST);

            // ---- Cards ----
            // Cards build their full world transform themselves,
            // so "world" goes back to identity.
            shader.setMat4("world", glm::mat4(1.0f));

            cardAnimator.draw(
                shader,
                cardTexture,
                cards,
                cardManager
            );

            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }

    glfwTerminate();

    return 0;
}