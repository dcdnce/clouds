#include "class_engine.h"
#include "class_logger.h"
#include <iostream>

Engine::Engine(void) : framebuffer_width(0), framebuffer_height(0), window(NULL)
{
}

Engine::~Engine(void)
{
	if (window)
		glfwDestroyWindow(this->window);
	glfwTerminate();
}

void	Engine::KeyCallback(GLFWwindow *w, int key, int scancode, int action, int mods)
{
	Engine* engine = static_cast<Engine*>(glfwGetWindowUserPointer(w));

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(w, true);
	if (key == GLFW_KEY_W && action == GLFW_REPEAT | GLFW_PRESS)
		engine->camera.ProcessKeyboard(FORWARD, engine->delta_time);
	if (key == GLFW_KEY_S && action == GLFW_REPEAT | GLFW_PRESS)
		engine->camera.ProcessKeyboard(BACKWARD, engine->delta_time);
	if (key == GLFW_KEY_A && action == GLFW_REPEAT | GLFW_PRESS)
		engine->camera.ProcessKeyboard(LEFT, engine->delta_time);
	if (key == GLFW_KEY_D && action == GLFW_REPEAT | GLFW_PRESS)
		engine->camera.ProcessKeyboard(RIGHT, engine->delta_time);
}

void Engine::MouseCallback(GLFWwindow* w, double current_mouse_x, double current_mouse_y)
{
	Engine* engine = static_cast<Engine*>(glfwGetWindowUserPointer(w));
	static bool first_mouse = true;
	static float last_mouse_x;
	static float last_mouse_y;
    float xpos = static_cast<float>(current_mouse_x);
    float ypos = static_cast<float>(current_mouse_y);

    if (first_mouse)
    {
        last_mouse_x = xpos;
        last_mouse_y = ypos;
        first_mouse = false;
    }

    float xoffset = xpos - last_mouse_x;
    float yoffset = last_mouse_y - ypos; // reversed since y-coordinates go from bottom to top
    last_mouse_x = xpos;
    last_mouse_y = ypos;

	engine->camera.ProcessMouseMovement(xoffset, yoffset);
}

void Engine::ComputeDeltaTime()
{
	float	current_frame = glfwGetTime();
	delta_time = current_frame - last_frame;
	last_frame = current_frame;
}