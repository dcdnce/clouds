/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pforesti <pforesti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/03 11:52:37 by difool            #+#    #+#             */
/*   Updated: 2023/07/02 15:09:00 by pforesti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "glad/glad.h"
#include "pfm/pfm.hpp"
#include <cmath>
#include <iostream>
#include <string>
#include "main.hpp"
#include "Engine.hpp"
#include "Logger.hpp"
#include "Shader.hpp"


float Noise(int x, int y, int random);

int	main(void)
{
	Engine 	clouds;
	Shader  shader;

	// Initialization
	clouds.init();
	shader.loadShaders("./shaders/vertex.glsl", "./shaders/frag.glsl");

	// Noise related
	float map32[32 * 32];
	for (size_t i = 0 ; i < 32 ; i++)
		for (size_t j = 0 ; j < 32 ; j++)
			map32[i*32+j] = Noise(j, i, 1336);

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
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, 32, 32, 0, GL_RED, GL_FLOAT, map32);
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

float Noise(int x, int y, int random)
{
    int n = x + y * 57 + random * 131;
    n = (n<<13) ^ n;
    return (1.0f - ( (n * (n * n * 15731 + 789221) +
            1376312589)&0x7fffffff)* 0.000000000931322574615478515625f);
}