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
#include "class_plane.h"
#include "class_camera.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

int	main(void)
{
	Engine 	clouds;
	Skydome skydome;
 
	// Initialization
	clouds.Init();
	clouds.camera.position = pfm::vec3(5.f, 6005.f, 20.f);
	skydome.shader.LoadShaders("./shaders/skydome.vert", "./shaders/skydome.frag");
	skydome.ComputePositions(6381.f, 60, 60);
	skydome.ComputeTexCoords();
	skydome.SendBuffers();
	skydome.shader.SetProjMat(pfm::perspective(pfm::radians(90.f), (float)W_WIDTH/(float)W_HEIGHT, 0.1f, 10000.f));
	skydome.InitDepthMap();
	skydome.depth_map_shader.SetProjMat(pfm::perspective(pfm::radians(90.f), 1024.f/1024.f, 0.1f, 10000.f));

	// Plane
	Plane plane;
	plane.shader.LoadShaders("./shaders/plane.vert", "./shaders/plane.frag");
	plane.shader.SetProjMat(
	    pfm::perspective(pfm::radians(90.f), (float)W_WIDTH/(float)W_HEIGHT, 0.1f, 1000.f)
	);
		// Debug Plane
		Plane debug_plane;
		debug_plane.Debug();
		debug_plane.shader.LoadShaders("./shaders/debug_plane.vert", "./shaders/debug_plane.frag");
		debug_plane.shader.SetProjMat(
			pfm::perspective(pfm::radians(90.f), (float)W_WIDTH/(float)W_HEIGHT, 0.1f, 10000.f)
		);
 
	// Main loop
	while (!glfwWindowShouldClose(clouds.window)) {
		clouds.ComputeDeltaTime();
		static int frames = 0;
		ImGui_ImplGlfw_NewFrame();
		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();
		// ImGui::ShowDemoWindow(); // Show demo window! :)
		clouds.Gui();

		skydome.DrawDepthMap(frames, clouds);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, W_WIDTH, W_HEIGHT);
		glClearColor(0.f, 0.f, 0.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Matrices - model and view
		skydome.shader.SetModelMat(pfm::mat4(1.f));
		skydome.shader.SetViewMat(clouds.camera.GetViewMatrix());
		
		plane.shader.SetModelMat(pfm::mat4(1.f));
		plane.shader.SetViewMat(clouds.camera.GetViewMatrix());

		// Draw
		skydome.Draw(frames, clouds);
		glUseProgram(plane.shader.program);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, skydome.depth_map_texture);
		glUniform1i(glGetUniformLocation(plane.shader.program, "texture_depth"), 0);
		plane.Draw(frames, clouds, skydome.depth_map_shader.GetProjMat(), skydome.depth_map_shader.GetViewMat());

			//Debug plane
			debug_plane.shader.SetModelMat(pfm::mat4(1.f));
			debug_plane.shader.SetViewMat(clouds.camera.GetViewMatrix());
			glUseProgram(debug_plane.shader.program);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, skydome.depth_map_texture);
			glUniform1i(glGetUniformLocation(debug_plane.shader.program, "texture1"), 0);
			glUseProgram(0);
			debug_plane.Draw(frames, clouds);

		ImGui::Render();// save DrawData
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData()); //send drawdata
		glfwSwapBuffers(clouds.window);
		glfwPollEvents();
		frames++;
	}

	return (0);
}
