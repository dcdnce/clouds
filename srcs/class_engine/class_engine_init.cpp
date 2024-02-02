#include <iostream>
#include "main.h"
#include "class_engine.h"
#include "class_logger.h"

void Engine::Init(void)
{
	glfwInit();
	_InitWindow();
	_InitGlad();
	_InitEvents();
	_InitGl();
}

void	Engine::_InitWindow()
{
	//Create a window
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	#ifdef __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	#endif

    // const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    // window = glfwCreateWindow(mode->width, mode->height, "clouds", glfwGetPrimaryMonitor(), NULL);
	window = glfwCreateWindow(W_WIDTH, W_HEIGHT, "clouds", NULL, NULL);
	if (window == NULL)
		throw std::runtime_error("Failed to create glfw window");

	glfwMakeContextCurrent(window);

	glfwGetFramebufferSize(window, &framebuffer_width, &framebuffer_height);
}

void	Engine::_InitEvents()
{
	glfwSetWindowUserPointer(window, this);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, Engine::MouseCallback);
	if (glfwRawMouseMotionSupported())
		glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

	glfwSetKeyCallback(window, Engine::KeyCallback);
}

void	Engine::_InitGl()
{
	glViewport(0, 0, framebuffer_width, framebuffer_height);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glPointSize(1.f);
}

void	Engine::_InitGlad()
{
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		throw std::runtime_error("Failed to initialize glad");
}
