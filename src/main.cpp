#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Cards/Cards.h"
#include "Graphics/Texture.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <cctype>
#include <filesystem>
#include <iostream>
#include <memory>
#include <vector>
#include <algorithm>

#include "Graphics/Quad.h"
#include "Graphics/Shader.h"
#include "Table/Table.h"
#include "Table/TableLegs.h"
#include "Room/Room.h"
#include "Room/RoomDecor.h"
#include "Graphics/Model.h"
#include "Cards/CardManager.h"
#include "Cards/CardAnimator.h"
#include "Camera/DualCamera.h"
#include "WorldConfig.h"

#ifndef ASSET_DIR
#define ASSET_DIR "./"
#endif


static std::vector<std::unique_ptr<Texture>> loadCardTextures(
    const std::string& folder
)
{
    namespace fs = std::filesystem;

    std::vector<std::string> files;

    if (fs::is_directory(folder))
    {
        for (const auto& entry : fs::directory_iterator(folder))
        {
            std::string extension = entry.path().extension().string();

            std::transform(
                extension.begin(), extension.end(), extension.begin(),
                [](unsigned char c) { return std::tolower(c); }
            );

            if (extension == ".png" || extension == ".jpg"
                || extension == ".jpeg")
            {
                files.push_back(entry.path().string());
            }
        }
    }

    std::sort(files.begin(), files.end());

    if (files.empty())
    {
        std::cerr << "No card images found in " << folder
                  << " - using wood.png for every card\n";
        files.push_back(ASSET_DIR "textures/wood.png");
    }

    std::vector<std::unique_ptr<Texture>> textures;

    for (int card = 1; card <= 5; ++card)
    {
        const std::string& path = files[(card - 1) % files.size()];

        std::cout << "Card " << card << " -> " << path << "\n";
        textures.push_back(std::make_unique<Texture>(path));
    }

    return textures;
}

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

        std::vector<std::unique_ptr<Texture>> cardTextures =
            loadCardTextures(ASSET_DIR "textures/cards/");

        Texture woodTexture(
            ASSET_DIR "textures/wood.png"
        );

        TableLegs tableLegs;

        Room room;

        RoomDecor roomDecor;

        Model adventurer(
            ASSET_DIR "models/Adventurer.glb",
            "CharacterArmature|Idle_Neutral",
            0.0f     // standing, arms down
        );

        glm::mat4 adventurerPlacement(1.0f);
        adventurerPlacement = glm::translate(
            adventurerPlacement,
            glm::vec3(0.0f, world::FLOOR_Y, -1.95f)
        );
        adventurerPlacement = glm::rotate(
            adventurerPlacement,
            glm::radians(0.0f),     
            glm::vec3(0.0f, 1.0f, 0.0f)
        );
        adventurerPlacement = glm::scale(
            adventurerPlacement,
            glm::vec3(1.7f)   // size: bigger number = bigger character
        );

        Cards cards;

        CardManager cardManager;
        CardAnimator cardAnimator;

        DualCamera camera;

        bool gWasPressed = false;

        float lastFrame = 0.0f;

        while (!glfwWindowShouldClose(window))
        {
            float currentFrame = static_cast<float>(glfwGetTime());

            float deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;

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
                glm::radians(55.0f),  
                aspect,
                0.1f,
                100.0f
            );

            shader.use();
            shader.setMat4("projection", projection);
            shader.setMat4("view", camera.getViewMatrix());

            // ---- Room ----
           
            shader.setMat4("world", glm::mat4(1.0f));
            room.draw(shader, camera.currentMode == 2);
            roomDecor.draw(shader, currentFrame);
            adventurer.draw(shader, adventurerPlacement);

            // ---- Legs ----
            
            shader.setMat4("world", glm::mat4(1.0f));
            tableLegs.draw(shader, woodTexture);

            // ---- Table ----
           
            shader.setMat4("world", world::tableToWorld());

            glDisable(GL_DEPTH_TEST);
            table.draw(shader, quad);
            glEnable(GL_DEPTH_TEST);

            // ---- Cards ----
           
            shader.setMat4("world", glm::mat4(1.0f));

            cardAnimator.draw(
                shader,
                cardTextures,
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