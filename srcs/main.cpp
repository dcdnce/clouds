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
#include "class_debug_plane.h"
#include "class_terrain.h"
#include "class_camera.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"


void	load_texture_png(GLuint* texture, const char* filename, GLenum activeTexture);

int	main(void)
{
	Engine 	engine;
	Skydome skydome;

	// Initialization
	engine.Init();
	engine.camera.position = pfm::vec3(9.f, 6032.f, 10.f);
	skydome.shader.LoadShaders("./shaders/skydome.vert", "./shaders/skydome.frag");
	skydome.ComputePositions(6381.f, 60, 60);
	skydome.ComputeTexCoords();
	skydome.SendBuffers();
	skydome.shader.SetProjMat(pfm::perspective(pfm::radians(90.f), (float)W_WIDTH / (float)W_HEIGHT, 0.1f, 10000.f));
	skydome.InitDepthMap();
	skydome.depth_map_shader.SetProjMat(pfm::perspective(pfm::radians(90.f), 1024.f / 1024.f, 0.1f, 100000.f));
	// skydome.depth_map_shader.SetProjMat(pfm::orthographic(-10.f, 10.f, -10.f, 10.f, 0.1f, 10000.f));

	// Debug Plane
	Plane debug_plane;
	debug_plane.Debug();
	debug_plane.shader.LoadShaders("./shaders/debug_plane.vert", "./shaders/debug_plane.frag");
	debug_plane.shader.SetProjMat(pfm::perspective(pfm::radians(90.f), (float)W_WIDTH / (float)W_HEIGHT, 0.1f, 10000.f));

	// Terrain
	Terrain terrain(1000);
	terrain.shader.LoadShaders("./shaders/terrain.vert", "./shaders/terrain.frag");
	terrain.SetupBuffers();
	terrain.shader.SetProjMat(pfm::perspective(pfm::radians(90.f), (float)W_WIDTH / (float)W_HEIGHT, 0.1f, 10000.f));
	// terrain.shader.SetProjMat(pfm::orthographic(-1000.f, 1000.f, -1000.f, 1000.f, 0.1f, 10000.f));

	// Main loop
	while (!glfwWindowShouldClose(engine.window)) {
		engine.ComputeDeltaTime();
		static int frames = 35000;
		// static int frames = 0;
		ImGui_ImplGlfw_NewFrame();
		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();
		engine.Gui();

		// Various data
		int sun_frames = 35000 - (frames % 70000); // E to W loop
		pfm::mat4 rotated_sun_mat = pfm::rotate(pfm::mat4(1.f), static_cast<float>(sun_frames) * pfm::radians(0.001), pfm::vec3(0.f,
		                                        0.f, 1.f));
		float zenith = static_cast<float>(pfm::magnitude(engine.camera.position - pfm::vec3(0.f, skydome.radius, 0.f)));

		// Depth maps
		skydome.DrawDepthMap(sun_frames, engine);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, W_WIDTH, W_HEIGHT);
		glClearColor(0.f, 0.f, 0.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Matrices - model and view
		skydome.shader.SetModelMat(pfm::mat4(1.f));
		skydome.shader.SetViewMat(engine.camera.GetViewMatrix());
		terrain.shader.SetModelMat(pfm::mat4(1.f));
		terrain.shader.SetViewMat(engine.camera.GetViewMatrix());

		// Draw
		skydome.Draw(frames, engine, rotated_sun_mat, zenith);
		glUseProgram(terrain.shader.program); // bin depth map texture to terrain shader
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, skydome.depth_map_texture);
		glUniform1i(glGetUniformLocation(skydome.shader.program, "texture_depth"), 0);
		glUseProgram(0);
		terrain.Draw(frames, engine, skydome.depth_map_shader.GetProjMat(), skydome.depth_map_shader.GetViewMat(), zenith,
		             rotated_sun_mat);

		//Debug plane
		debug_plane.shader.SetModelMat(pfm::mat4(1.f));
		debug_plane.shader.SetViewMat(engine.camera.GetViewMatrix());
		glUseProgram(debug_plane.shader.program);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, skydome.depth_map_texture);
		glUniform1i(glGetUniformLocation(debug_plane.shader.program, "texture1"), 0);
		glUseProgram(0);
		debug_plane.Draw(frames, engine);

		ImGui::Render();// save DrawData
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData()); //send drawdata
		glfwSwapBuffers(engine.window);
		glfwPollEvents();
		frames++;
		// frames += 10;
		frames += 50;
	}

	return (0);
}

void	load_texture_png(GLuint* texture, const char* filename, GLenum activeTexture)
{
	glGenTextures(1, texture);
	glActiveTexture(activeTexture);
	glBindTexture(GL_TEXTURE_2D, *texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// stbi Load
	unsigned char*	data;
	int				width, height, nbrChannels;
	data = stbi_load(filename, &width, &height, &nbrChannels, 0);
	if (data) {
		/* Add(!) & Configure texture	*/
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "main.cpp::stbi_load::couldn't load texture into data\n" << std::endl;
	}
	stbi_image_free(data);
	glBindTexture(GL_TEXTURE_2D, 0);
}