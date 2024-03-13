#include "class_engine.h"
#include "class_logger.h"
#include <iostream>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

bool isImGuiMouseCaptured = true;

Engine::Engine(void) : framebuffer_width(0), framebuffer_height(0), window(NULL)
{
	sun_position = pfm::vec3(0.0, 1000000.0, 0.0);
	clouds_type = TYPE_CUMULUS;
	average_density = true;
	average_density_step_size = 10.f;
	optical_length_air = 8.4f;
	optical_length_haze = 1.25f;
	clouds_render = true;
	CloudsTypeCallback();
}

Engine::~Engine(void)
{
	if (window)
		glfwDestroyWindow(this->window);
	glfwTerminate();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

}

void	Engine::KeyCallback(GLFWwindow* w, int key, int scancode, int action, int mods) noexcept
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
	if (key == GLFW_KEY_TAB && action == GLFW_PRESS) {
          isImGuiMouseCaptured = !isImGuiMouseCaptured;
		if (isImGuiMouseCaptured) {
            glfwSetInputMode(w, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        } else {
            glfwSetInputMode(w, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
	}

	ImGui_ImplGlfw_MouseButtonCallback(w, key, action, mods);
	ImGui_ImplGlfw_KeyCallback(w, key, scancode, action, mods);
}

void Engine::MouseCallback(GLFWwindow* w, double current_mouse_x, double current_mouse_y) noexcept
{
	Engine* engine = static_cast<Engine*>(glfwGetWindowUserPointer(w));
	if (!isImGuiMouseCaptured && !ImGui::GetIO().WantCaptureMouse) {
	// if (!isImGuiMouseCaptured) {
		static bool first_mouse = true;
		static float last_mouse_x;
		static float last_mouse_y;
		float xpos = static_cast<float>(current_mouse_x);
		float ypos = static_cast<float>(current_mouse_y);

		if (first_mouse) {
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
	else {
		ImGui_ImplGlfw_CursorPosCallback(w, current_mouse_x, current_mouse_y);
	}
}

void Engine::CloudsTypeCallback()
{
	if (clouds_type & TYPE_CUMULUS) {
		clouds_smoothstep_edge_min = 0.950f;
		clouds_smoothstep_edge_max = 1.150f;
		noise_scale = 23.f;
	}

	if (clouds_type & TYPE_CIRRUS) {
		clouds_smoothstep_edge_min = 0.7f;
		clouds_smoothstep_edge_max = 2.3f;
		noise_scale = 15.f;
	}
}