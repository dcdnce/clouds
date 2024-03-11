#pragma once
#include <vector>
#include "glad/glad.h"
#include "class_vertex.h"
#include "class_shader.h"
#include "class_engine.h"

class Plane {
	private:
		GLuint _VAO, _VBO, _EBO;
		std::vector<Vertex> _vertices;
		std::vector<GLuint> _indices;
		void _SetupBuffers();
	public:
	 	Shader shader;
		Plane();
		~Plane();
		void Debug();
		inline void Draw(int const frames, Engine & e)
		{
			glUseProgram(shader.program);
			pfm::mat4 rotated_sun_mat = pfm::rotate(pfm::mat4(1.f), static_cast<float>(frames) * pfm::radians(0.01), pfm::vec3(0.f, 0.f, 1.f));
			glUniformMatrix4fv(glGetUniformLocation(shader.program, "uRotatedSun"), 1, GL_FALSE, &rotated_sun_mat);
			glUniform3f(glGetUniformLocation(shader.program, "uSunPosition"), e.sun_position.x, e.sun_position.y, e.sun_position.z);

			glBindVertexArray(_VAO);
			glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
			glUseProgram(0);
		}

		inline void DrawWith(Shader & new_shader)
		{
			glUseProgram(new_shader.program);
			glBindVertexArray(_VAO);
			glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
			glUseProgram(0);

		}
};