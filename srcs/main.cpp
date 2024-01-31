#include "glad/glad.h"
#include "pfm/pfm.hpp"
#include <cmath>
#include <iostream>
#include <string>
#include "main.h"
#include "class_engine.h"
#include "class_logger.h"
#include "class_shader.h"
#include "class_skydome.h"

int	main(void)
{
	Engine 	clouds;
	Shader  shader;
	Skydome skydome;

	// Initialization
	clouds.Init();
	clouds.camera.position = pfm::vec3(0.f, 5.f, 0.f);
	shader.LoadShaders("./shaders/panel.vs", "./shaders/panel.fs");
	skydome.shader.LoadShaders("./shaders/skydome.vert", "./shaders/skydome.frag");
	skydome.ComputePositions(10.f, 60, 60);
	skydome.ComputeTexCoords();
	skydome.SendBuffers();
	skydome.CreateTexture();
	skydome.NoiseInterpolation(1998);

	// Noise panel
	GLfloat vertices[] = {
		// Position       // TexCoord
		-10.0f, -10.0f,     0.0f, 0.0f,
		 10.0f, -10.0f,     1.0f, 0.0f,
		 10.0f,  10.0f,     1.0f, 1.0f,
		 -10.0f,  10.0f,    0.0f, 1.0f
	    };
	GLuint indices[] = {
		0, 1, 3,
		1, 2, 3
	};

	// Panel texture
	GLuint texture_id;
	glUseProgram(shader.program);
	glGenTextures(1, &texture_id);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(glGetUniformLocation(shader.program, "texture1"), 0);
	glUseProgram(0);

	/* Buffers */
	GLuint vao;
	GLuint vbo;
	GLuint ebo;
	// Bind and fill objects
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	// Link the data
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
	// Unbind
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// Set projection matrix
	shader.SetProjMat(
	    pfm::perspective(pfm::radians(90.f), (float)W_WIDTH/(float)W_HEIGHT, 0.1f, 1000.f)
	);
	skydome.shader.SetProjMat(
	    pfm::perspective(pfm::radians(90.f), (float)W_WIDTH/(float)W_HEIGHT, 0.1f, 1000.f)
	);


	// Main loop
	while (!glfwWindowShouldClose(clouds.window)) {
		clouds.ComputeDeltaTime();
		static int frames = 0;
		// glClearColor(0.16f, 0.32f, 0.75f, 1.f);
		glClearColor(0.f, 0.f, 0.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Matrices - model and view
		shader.SetModelMat(pfm::mat4(1.f));
		shader.SetViewMat(clouds.camera.GetViewMatrix());
		skydome.shader.SetModelMat(pfm::mat4(1.f));
		skydome.shader.SetViewMat(clouds.camera.GetViewMatrix());

		// Draw skydome
		skydome.Draw(frames, clouds.camera.position);

		// // Draw noise panel
		// glUseProgram(shader.program);
		// glBindVertexArray(vao);

		// glActiveTexture(GL_TEXTURE0);
		// glBindTexture(GL_TEXTURE_2D, texture_id);
		// glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, 256, 256, 0, GL_RED, GL_UNSIGNED_BYTE, skydome.texture);

		// glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// glBindVertexArray(0);
		// glUseProgram(0);

		glfwSwapBuffers(clouds.window);
		glfwPollEvents();
		frames++;
	}

	return (0);
}
