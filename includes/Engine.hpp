#pragma once

#include <glad/glad.h>
#include <glfw/glfw3.h>

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

		int			framebufferWidth;
		int			framebufferHeight;
		GLFWwindow* window;
};

