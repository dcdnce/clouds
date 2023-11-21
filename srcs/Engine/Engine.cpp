#include "Engine.hpp"
#include "Logger.hpp"
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
}