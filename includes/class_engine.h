#pragma once

#include <glad/glad.h>
#include <glfw/glfw3.h>
#include "class_camera.h"
#include "pfm/pfm.hpp"
#include "imgui.h"

#define TYPE_CUMULUS	0b0001
#define TYPE_CIRRUS		0b0010

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
		void CloudsTypeCallback();
		inline void ComputeDeltaTime() noexcept
		{
			float	current_frame = glfwGetTime();
			delta_time = current_frame - last_frame;
			last_frame = current_frame;
		}
		void Gui();

		Camera camera;
		pfm::vec3 sun_position;
		float noise_scale;
		int clouds_type;
		float clouds_smoothstep_edge_min;
		float clouds_smoothstep_edge_max;
		bool clouds_render;
		bool average_density;
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

