#include "class_engine.h"
#include "class_logger.h"
#include <iostream>

Engine::Engine(void) : framebufferWidth(0), framebufferHeight(0), window(NULL)
{
}

Engine::~Engine(void)
{
	if (window)
		glfwDestroyWindow(this->window);
	glfwTerminate();
}

void	Engine::keyCallback(GLFWwindow *w, int key, int scancode, int action, int mods)
{
	Engine* engine = static_cast<Engine*>(glfwGetWindowUserPointer(w));

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(w, true);
	if (key == GLFW_KEY_W && action == GLFW_REPEAT | GLFW_PRESS)
		engine->camera.processKeyboard(FORWARD, engine->deltaTime);
	if (key == GLFW_KEY_S && action == GLFW_REPEAT | GLFW_PRESS)
		engine->camera.processKeyboard(BACKWARD, engine->deltaTime);
	if (key == GLFW_KEY_A && action == GLFW_REPEAT | GLFW_PRESS)
		engine->camera.processKeyboard(LEFT, engine->deltaTime);
	if (key == GLFW_KEY_D && action == GLFW_REPEAT | GLFW_PRESS)
		engine->camera.processKeyboard(RIGHT, engine->deltaTime);
}

void Engine::mouseCallback(GLFWwindow* w, double currMouseX, double currMouseY)
{
	Engine* engine = static_cast<Engine*>(glfwGetWindowUserPointer(w));
	static bool firstMouse = true;
	static float lastMouseX;
	static float lastMouseY;
    float xpos = static_cast<float>(currMouseX);
    float ypos = static_cast<float>(currMouseY);

    if (firstMouse)
    {
        lastMouseX = xpos;
        lastMouseY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastMouseX;
    float yoffset = lastMouseY - ypos; // reversed since y-coordinates go from bottom to top
    lastMouseX = xpos;
    lastMouseY = ypos;

	engine->camera.processMouseMovement(xoffset, yoffset);
}

void Engine::computeDeltaTime()
{
	float	currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
}