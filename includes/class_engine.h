#pragma once

#include <glad/glad.h>
#include <glfw/glfw3.h>
#include "class_camera.h"

class Engine {
	private:
		void	_InitWindow();
		void	_InitGlad();
		void	_InitEvents();
		void	_InitGl();
	public:
		Engine(void);
		~Engine(void);
		void Init(void);
		static void	KeyCallback(GLFWwindow* w, int key, int scancode, int action, int mods);
		static void MouseCallback(GLFWwindow* w, double current_mouse_x, double current_mouse_y);
		void ComputeDeltaTime();

		Camera camera;
		float delta_time;
		float last_frame;
		int			framebuffer_width;
		int			framebuffer_height;
		GLFWwindow* window;
};

