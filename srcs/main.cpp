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

inline void	interpolate(unsigned char* buffer, int step) noexcept
{
	for (int y = 0 ; y < 256 ; y += step) {
		for (int x = 0 ; x < 256 ; x += step) {
			float a = buffer[(y<<8)+x];
			float b = buffer[(y<<8)+((x+step)&255)];
			float c = buffer[(((y+step)&255)<<8)+x];
			float d = buffer[(((y+step)&255)<<8)+((x+step)&255)];
			for (int i = 0 ; i < step ; i++) {
				for (int j = 0 ; j < step ; j++) {
					float xf = (float)j / (float)step;
					float yf = (float)i / (float)step;
					xf = xf*xf*(3.f-2.f*xf);
					yf = yf*yf*(3.f-2.f*yf);
					float nx0 = a*(1.f-xf)+b*xf;
					float nx1 = c*(1.f-xf)+d*xf;
					buffer[(((y+i)<<8))+(x+j)] = nx0*(1.f-yf)+nx1*yf;
				}
			}
		}
	}
}

int	main(void)
{
	Engine 	clouds;
	Shader  shader;
	Skydome skydome;

	// Initialization
	clouds.Init();
	shader.LoadShaders("./shaders/vertex.glsl", "./shaders/frag.glsl");
	skydome.shader.LoadShaders("./shaders/skydome.vs", "./shaders/skydome.fs");
	skydome.ComputePositions(10.f, 100, 100);
	skydome.ComputeTexCoords();
	skydome.SendBuffers();

	// Noise related
	unsigned char tex[8][256*256];
	// For every octave fill the texture and interpolate
	srand(2009);
	for (int i = 0 ; i < 8 ; i++) {
		for (int j = 0 ; j < 256*256 ; j++)
			tex[i][j] = rand()&255;
		interpolate(tex[i], 1<<i);
	}

	/* Texture */
	// Creer et bind texture
	glUseProgram(skydome.shader.program);
	GLuint	texID;
	glGenTextures(1, &texID);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, 256, 256, 0, GL_RED, GL_UNSIGNED_BYTE, tex[3]);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(glGetUniformLocation(skydome.shader.program, "texture1"), 0);
	glUseProgram(0);

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
		glClearColor(0.16f, 0.32f, 0.75f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//offsets
		int offsets[8];
		int t = frames;
		offsets[0] = static_cast<int>(t * 0.025f) << 8;
		offsets[1] = static_cast<int>(t * 0.030f);
		offsets[2] = static_cast<int>(-t * 0.035f);
		offsets[3] = static_cast<int>(t * 0.040f) << 8;
		offsets[4] = static_cast<int>(t * 0.045f);
		offsets[5] = static_cast<int>(-t * 0.050f);
		offsets[6] = static_cast<int>(t * 0.055f) << 8;
		offsets[7] = static_cast<int>(-t * 0.060f);

		//composition
		unsigned char buffer[256*256];
		for (int i = 0 ; i < 65536 ; i++) {
			int sum = 0;
			for (int k = 0 ; k < 8 ; k++) {
				int index = ((i + offsets[k]) & 0x0000ffff);
				sum += (tex[k][index] << k);
			}
			//sum += 128; 
			buffer[i] = sum >> 8;
		}

		// Matrices - model and view
		shader.SetModelMat(pfm::mat4(1.f));
		shader.SetViewMat(clouds.camera.GetViewMatrix());
		skydome.shader.SetModelMat(pfm::mat4(1.f));
		skydome.shader.SetViewMat(clouds.camera.GetViewMatrix());

		// Draw skydome
		glUseProgram(skydome.shader.program);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, 256, 256, 0, GL_RED, GL_UNSIGNED_BYTE, buffer);
		glUseProgram(0);
		skydome.Draw();

//		// Draw noise panel
//		glUseProgram(shader.program);
//		glBindVertexArray(vao);
//
//		glActiveTexture(GL_TEXTURE0);
//		glBindTexture(GL_TEXTURE_2D, texID);
//		glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, 256, 256, 0, GL_RED, GL_UNSIGNED_BYTE, buffer);
//
//		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
//
//		glBindVertexArray(0);
//		glUseProgram(0);

		glfwSwapBuffers(clouds.window);
		glfwPollEvents();
		frames++;
	}

	return (0);
}
