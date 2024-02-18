#include <cmath>
#include <iostream>
#include <string>
#include "glad/glad.h"
#include "pfm/pfm.hpp"
#include "main.h"
#include "class_engine.h"
#include "class_logger.h"
#include "class_shader.h"
#include "class_skydome.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

int	main(void)
{
	Engine 	clouds;
	Skydome skydome;

	// Initialization
	clouds.Init();
	clouds.camera.position = pfm::vec3(0.f, 6000.f, 0.f);
	skydome.shader.LoadShaders("./shaders/skydome.vert", "./shaders/skydome.frag");
	skydome.ComputePositions(6381.f, 60, 60);
	skydome.ComputeTexCoords();
	skydome.SendBuffers();

	skydome.shader.SetProjMat(
	    pfm::perspective(pfm::radians(90.f), (float)W_WIDTH/(float)W_HEIGHT, 0.1f, 10000.f)
	);

	// Main loop
	while (!glfwWindowShouldClose(clouds.window)) {
		static int frames = 0;
		ImGui_ImplGlfw_NewFrame();
		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();

		// ImGui::ShowDemoWindow(); // Show demo window! :)
		clouds.Gui();

		clouds.ComputeDeltaTime();
		glClearColor(0.f, 0.f, 0.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Matrices - model and view
		skydome.shader.SetModelMat(pfm::mat4(1.f));
		skydome.shader.SetViewMat(clouds.camera.GetViewMatrix());

		// Draw skydome
		skydome.Draw(frames, clouds);

		ImGui::Render();// save DrawData
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData()); //send drawdata
		glfwSwapBuffers(clouds.window);
		glfwPollEvents();
		frames++;
	}

	return (0);
}
