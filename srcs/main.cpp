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
	// clouds.camera.position = pfm::vec3(0.f, 6000.f, 15.f);
	// clouds.camera.front = pfm::vec3(0.f, -1.f, 0.f);
	// clouds.camera.up = pfm::vec3(-1.f, 0.f, 0.f);
	skydome.shader.LoadShaders("./shaders/skydome.vert", "./shaders/skydome.frag");
	skydome.ComputePositions(6381.f, 60, 60);
	skydome.ComputeTexCoords();
	skydome.SendBuffers();
	skydome.shader.SetProjMat(
	    pfm::perspective(pfm::radians(90.f), (float)W_WIDTH/(float)W_HEIGHT, 0.1f, 10000.f)
	);

	// Plane
	Plane plane;
	plane.shader.LoadShaders("./shaders/plane.vert", "./shaders/plane.frag");
	plane.shader.SetProjMat(
	    pfm::perspective(pfm::radians(90.f), (float)W_WIDTH/(float)W_HEIGHT, 0.1f, 10000.f)
	);

		/* SHADOW MAP */
		GLuint depth_map_FBO;
		glGenFramebuffers(1, &depth_map_FBO);
		// Texture
		GLuint depth_map;
		glGenTextures(1, &depth_map);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, depth_map);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); 
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); 
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);
		// Attach texture to framebuffer as depth buffer
		glBindFramebuffer(GL_FRAMEBUFFER, depth_map_FBO);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth_map, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		// Shader
		Shader depth_map_shader;
		depth_map_shader.LoadShaders("./shaders/sm.vert", "./shaders/sm.frag");
		depth_map_shader.SetProjMat(
			pfm::perspective(pfm::radians(90.f), 1024.f/1024.f, 0.1f, 10000.f)
		);
		depth_map_shader.SetViewMat(pfm::lookAt(pfm::vec3(0.f, 7000.f, 15.f), pfm::vec3(0.f, -1.f, 0.f), pfm::vec3(1.f, 0.f, 0.f)));
		depth_map_shader.SetModelMat(pfm::mat4(1.f));
		// Debug Plane
		Plane debug_plane;
		debug_plane.Debug();
		debug_plane.shader.LoadShaders("./shaders/debug_plane.vert", "./shaders/debug_plane.frag");
		debug_plane.shader.SetProjMat(
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

			/* SHADOW MAP */
			glViewport(0, 0, 1024, 1024);
			glBindFramebuffer(GL_FRAMEBUFFER, depth_map_FBO);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glDepthMask(GL_TRUE);
			glEnable(GL_DEPTH_TEST);
			depth_map_shader.SetViewMat(clouds.camera.GetViewMatrix());
			// depth_map_shader.SetViewMat(pfm::lookAt(pfm::vec3(0.f, 6500.f, 15.f), pfm::vec3(0.f, -1.f, 0.f), pfm::vec3(1.f, 0.f, 0.f)));
			skydome.DrawWith(frames, clouds, depth_map_shader);
			plane.DrawWith(depth_map_shader);
			debug_plane.DrawWith(depth_map_shader);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, W_WIDTH, W_HEIGHT);
		glClearColor(0.f, 0.2f, 1.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		clouds.ComputeDeltaTime();

		// Matrices - model and view
		skydome.shader.SetModelMat(pfm::mat4(1.f));
		skydome.shader.SetViewMat(clouds.camera.GetViewMatrix());
		
		plane.shader.SetModelMat(pfm::mat4(1.f));
		plane.shader.SetViewMat(clouds.camera.GetViewMatrix());

		// Draw
		skydome.Draw(frames, clouds);
		plane.Draw(frames, clouds);

			//Debug plane
			debug_plane.shader.SetModelMat(pfm::mat4(1.f));
			debug_plane.shader.SetViewMat(clouds.camera.GetViewMatrix());
			glUseProgram(debug_plane.shader.program);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, depth_map);
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
