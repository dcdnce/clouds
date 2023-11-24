#include "glad/glad.h"
#include "pfm/pfm.hpp"
#include <cmath>
#include <iostream>
#include <string>
#include "main.hpp"
#include "Engine.hpp"
#include "Logger.hpp"
#include "Shader.hpp"

void	interpolate(unsigned char * buffer, int step)
{
	for (int y = 0 ; y < 256 ; y += step)
	{
		for (int x = 0 ; x < 256 ; x += step)
		{
			float a = buffer[(y<<8)+x];
			float b = buffer[(y<<8)+((x+step)&255)];
			float c = buffer[(((y+step)&255)<<8)+x]; 
			float d = buffer[(((y+step)&255)<<8)+((x+step)&255)]; 
			for (int i = 0 ; i < step ; i++)
			{
				for (int j = 0 ; j < step ; j++)
				{
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

	// Initialization
	clouds.init();
	shader.loadShaders("./shaders/vertex.glsl", "./shaders/frag.glsl");

	// Noise related
	unsigned char tex[8][256*256];
	// For every octave fill the texture and interpolate
	for (int i = 0 ; i < 8 ; i++)
	{
		for (int j = 0 ; j < 256*256 ; j++)
			tex[i][j] = rand()&255;
		interpolate(tex[i], 1<<i);
	}

	/* Texture */
	// Creer et bind texture
	glUseProgram(shader.program);
	GLuint	texID;
	glGenTextures(1, &texID);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, 256, 256, 0, GL_RED, GL_UNSIGNED_BYTE, tex[3]);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(glGetUniformLocation(shader.program, "texture1"), 0);
	glUseProgram(0);

	GLfloat vertices[] = {
        // Position       // TexCoord
        -1.0f, -1.0f,     0.0f, 0.0f,
         1.0f, -1.0f,     1.0f, 0.0f,
         1.0f,  1.0f,     1.0f, 1.0f,
        -1.0f,  1.0f,     0.0f, 1.0f
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


	// Main loop
	while (!glfwWindowShouldClose(clouds.window))
	{
		glClearColor(0.3f, 0.49f, 0.66f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		glUseProgram(shader.program);
		glBindVertexArray(vao);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texID);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glBindVertexArray(0);
		glUseProgram(0);

        glfwSwapBuffers(clouds.window);
        glfwPollEvents();
	}

	return (0);
}

