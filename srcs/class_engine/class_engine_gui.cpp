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
	ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
	ImGui::Text("TAB to change focus");

	// Infos header
	if (ImGui::CollapsingHeader("Infos")) {
		ImGui::SetNextItemWidth(150.f);
		ImGui::InputFloat3("Camera Position", &camera.position, "%.0f");
		ImGui::SetNextItemWidth(150.f);
		ImGui::InputFloat3("Sun Position", &sun_position, "%.0f");
	}

	if (ImGui::CollapsingHeader("Clouds")) {
		// ImGui::SeparatorText("Noise");
		ImGui::Checkbox("Render", &clouds_render);
		ImGui::SetNextItemWidth(100.f);
		int old_clouds_type = clouds_type;
		ImGui::RadioButton("Cumulus", &clouds_type, TYPE_CUMULUS);
		ImGui::SameLine();
		ImGui::RadioButton("Cirrus", &clouds_type, TYPE_CIRRUS);
		if (clouds_type != old_clouds_type) {
			CloudsTypeCallback();
		}
		ImGui::SetNextItemWidth(100.f);
		ImGui::DragFloat("Smoothstep Edge Min", &clouds_smoothstep_edge_min, 0.05f, -4.f, 4.f);
		ImGui::SetItemTooltip("Smoothstep redistributes noise value. The less the difference between the two values, the denser the clouds will appear.");
		ImGui::SetNextItemWidth(100.f);
		ImGui::DragFloat("Smoothstep Edge Max", &clouds_smoothstep_edge_max, 0.05f, -4.f, 4.f);
		ImGui::SetNextItemWidth(100.f);
		ImGui::DragFloat("Noise Scale", &noise_scale);
		ImGui::SetNextItemWidth(100.f);
		ImGui::Checkbox("Average Density", &average_density);
		ImGui::SetNextItemWidth(100.f);
		ImGui::DragFloat("Average Density Steps Size", &average_density_step_size);
	}

	if (ImGui::CollapsingHeader("Light")) {
		ImGui::SetNextItemWidth(100.f);
		ImGui::DragFloat("Air Optical Length", &optical_length_air, 0.1f, 0.f, 100.f);
		ImGui::SetNextItemWidth(100.f);
		ImGui::DragFloat("Haze Optical Length", &optical_length_haze, 0.1f, 0.f, 17.f);
		ImGui::SetNextItemWidth(100.f);
		ImGui::DragFloat("Mie phase eccentricity", &mie_phase_eccentricity, 0.01f, -9.f, 9.f);

	}

	ImGui::End();
}