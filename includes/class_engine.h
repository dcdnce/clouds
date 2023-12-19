#pragma once

#include <glad/glad.h>
#include <glfw/glfw3.h>
#include "class_camera.h"

class Engine
{
	private:
		void	_initWindow();
		void	_initGlad();
		void	_initEvents();
		void	_initGl();
	public:
		Engine(void);
		~Engine(void);
		void init(void);
		static void	keyCallback(GLFWwindow *w, int key, int scancode, int action, int mods);
		static void mouseCallback(GLFWwindow* w, double currMouseX, double currMouseY);
		void computeDeltaTime();

		Camera camera;
		float deltaTime;
		float lastFrame;
		int			framebufferWidth;
		int			framebufferHeight;
		GLFWwindow* window;
};

