#pragma once

#include <glad/glad.h>
#include <glfw/glfw3.h>
#include "class_camera.h"
#include "pfm/pfm.hpp"
#include "imgui.h"

class Engine {
	private:
		void	_InitWindow();
		void	_InitGlad();
		void    _InitImGui();
		void	_InitEvents();
		void	_InitGl();
	public:
		Engine();
		~Engine();
		void Init();
		static void	KeyCallback(GLFWwindow* w, int key, int scancode, int action, int mods) noexcept;
		static void MouseCallback(GLFWwindow* w, double current_mouse_x, double current_mouse_y) noexcept;
		inline void ComputeDeltaTime() noexcept
		{
			float	current_frame = glfwGetTime();
			delta_time = current_frame - last_frame;
			last_frame = current_frame;
		}
		void Gui();

		Camera camera;
		pfm::vec3 sun_position;
		float average_density_step_size;
		float optical_length_air;
		float optical_length_haze;
		float delta_time;
		float last_frame;
		int			framebuffer_width;
		int			framebuffer_height;
		GLFWwindow* window;
		// ImGuiIO& imgui_io;
};

