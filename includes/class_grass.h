#pragma once
#include <vector>
#include "glad/glad.h"
#include "class_vertex.h"
#include "class_shader.h"
#include "class_engine.h"

class Grass {
	private:
		GLuint _VAO, _VBO;
		std::vector<Vertex> _vertices;
		void _SetupBuffers();
	public:
	 	Shader shader;
		GLuint grass_texture;
		Grass();
		~Grass();
		inline void Draw(int const frames, Engine & e)
		{
			glUseProgram(shader.program);
			pfm::mat4 rotated_sun_mat = pfm::rotate(pfm::mat4(1.f), static_cast<float>(frames) * pfm::radians(0.001), pfm::vec3(0.f, 0.f, 1.f));
			glUniformMatrix4fv(glGetUniformLocation(shader.program, "uRotatedSun"), 1, GL_FALSE, &rotated_sun_mat);
			glUniform1i(glGetUniformLocation(shader.program, "uFrames"), frames);
			glUniform3f(glGetUniformLocation(shader.program, "uSunPosition"), e.sun_position.x, e.sun_position.y, e.sun_position.z);
			glBindVertexArray(_VAO);
			glDrawArrays(GL_POINTS, 0, _vertices.size());
			glBindVertexArray(0);
			glUseProgram(0);
		}
};