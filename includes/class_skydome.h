#pragma once

#include <iostream>
#include "glad/glad.h"
#include <vector>
#include "class_vertex.h"
#include "class_shader.h"

class Skydome {
	private:
		GLuint _VBO;
		GLuint _VAO;
		size_t _num_vertices;
		std::vector<Vertex> _vertices;
	public:
		Shader shader;

		Skydome();
		~Skydome();
		void ComputePositions(float const& radius, size_t const& num_rows, size_t const& num_cols);
		void ComputeTexCoords();
		void SendBuffers();
		inline void Draw()
		{
			glUseProgram(this->shader.program);
			glBindVertexArray(_VAO);

			glDrawArrays(GL_TRIANGLES, 0, _vertices.size());

			glUseProgram(0);
			glBindVertexArray(0);
		}
};
