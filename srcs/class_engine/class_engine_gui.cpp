#include "class_engine.h"

void Engine::Gui()
{
    ImGuiIO& io = ImGui::GetIO();

	const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + 650, main_viewport->WorkPos.y + 20), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(550, 680), ImGuiCond_FirstUseEver);
	if (!ImGui::Begin("Clouds", nullptr, 0)) {
		ImGui::End();
		return ;
	}

	// Framerate
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
    ImGui::Text("TAB to change focus");

	// Infos header
	if (ImGui::CollapsingHeader("Infos")) {
		ImGui::SetNextItemWidth(200.f);
		ImGui::InputFloat3("Camera Position", &camera.position, "%.0f");
		ImGui::SetNextItemWidth(200.f);
		ImGui::InputFloat3("Sun Position", &sun_position, "%.0f");
	}

	if (ImGui::CollapsingHeader("Shader Data")) {
		ImGui::SeparatorText("Noise");
		ImGui::SetNextItemWidth(200.f);
		ImGui::DragFloat("Average Density Steps Size", &average_density_step_size);
	}

	ImGui::End();
}