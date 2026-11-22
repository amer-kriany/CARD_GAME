// GLProject.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include "libs/glad_loader_docs.h"
#include <GLFW/glfw3.h>
#include <learnopengl/shader_m.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "Polygon.cpp"
#include "Table.h"
#include <learnopengl/camera.h>
#include "Room.h"
#include "stb_image.h"
#include "cards.h"

using namespace glm;

int SCR_WIDTH = 1280;
int SCR_HEIGHT = 720;

Camera camera(glm::vec3(0.0f, 2.0f, 5.0f)); // camera postion
float lastX = 400.0f;						// camera postion
float lastY = 300.0f;						// camera postion
float deltaTime = 0.0f;						// for camera movement
float lastFrame = 0.0f;						// for camera movement
bool firstMouse = true;

// method for camera movement
void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(
		xoffset,
		yoffset);
}
void processInput(GLFWwindow *window) // check input
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
}
unsigned int loadTexture(const char *path) // method for Texture
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrChannels;

	// stbi_set_flip_vertically_on_load(true);

	unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);

	if (data)
	{

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glBindTexture(GL_TEXTURE_2D, textureID);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Failed to load texture: " << path << std::endl;
	}

	return textureID;
}

int main()
{
	//  screen Preparation
	glfwInit();
	GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "my project 3", NULL, NULL);
	glfwMakeContextCurrent(window);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	glEnable(GL_DEPTH_TEST);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	stbi_set_flip_vertically_on_load(true);
	Shader ourShader("./shaders/vs/L3.vs", "./shaders/fs/L3.fs");

	Table table;
	Room room;
	Cards cards;

	unsigned int woodTexture = loadTexture("./textures/wood3.jpg"); // image for texture
	table.top.setTexture(woodTexture);								// set image in texture

	unsigned int a2 = loadTexture("./textures/wood7.jpg"); // image for texture
	room.ceiling.setTexture(a2);
	room.frontWall.setTexture(a2);
	room.backWall.setTexture(a2);
	room.leftWall.setTexture(a2);
	room.rightWall.setTexture(a2);

	unsigned int textureImage = loadTexture("./textures/r.jpg"); // image for texture
	cards.card1.setTexture(textureImage);
	textureImage = loadTexture("./textures/g1.jpg"); // image for texture
	cards.card2.setTexture(textureImage);
	textureImage = loadTexture("./textures/d.jpg");
	cards.card3.setTexture(textureImage);
	textureImage = loadTexture("./textures/a.jpg");
	cards.card4.setTexture(textureImage);
	textureImage = loadTexture("./textures/m.jpg");
	cards.card5.setTexture(textureImage);

	unsigned int a = loadTexture("./textures/wood6.jpg"); // image for texture
	room.floor.setTexture(a);

	unsigned int pic = loadTexture("./textures/g.jpg"); // image for texture
	room.pic.setTexture(pic);

	ourShader.use();
	// ourShader.setInt("texture1", 0);

	// setings for projection
	mat4 projection = mat4(1.0f);
	projection = perspective(radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	ourShader.setMat4("projection", projection);
	//-------

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// for camera movement
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// for draw
		glm::mat4 view = camera.GetViewMatrix();
		ourShader.setMat4("view", view);

		room.draw(ourShader);
		table.draw(ourShader);
		cards.draw(ourShader);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
