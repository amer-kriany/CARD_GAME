
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <algorithm>

#include "Graphics/Quad.h"
#include "Graphics/Shader.h"
#include "Table/Table.h"
#include "Cards/CardManager.h"
#include "Cards/CardAnimator.h"

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

    {
        Shader shader(
            ASSET_DIR "shaders/basic.vert",
            ASSET_DIR "shaders/basic.frag"
        );

        Quad quad;

        Table table(
            ASSET_DIR "textures/wood.png"
        );

        CardManager cardManager;
        CardAnimator cardAnimator;

        glm::mat4 projection = glm::ortho(
            -1.0f,
            1.0f,
            -0.75f,
            0.75f,
            -1.0f,
            1.0f
        );

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

            cardAnimator.update(deltaTime);

            glClearColor(
                0.025f,
                0.018f,
                0.015f,
                1.0f
            );

            glClear(GL_COLOR_BUFFER_BIT);

            shader.use();
            shader.setMat4("projection", projection);

            // Draw the table first.
            table.draw(shader, quad);

            // Draw the deck, played cards, and active animation.
            cardAnimator.draw(
                shader,
                quad,
                cardManager
            );

            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }

    glfwTerminate();

    return 0;
}
