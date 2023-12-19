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
		void FillBuffers(float const& radius, size_t const& num_rows, size_t const& num_cols);
		void SendBuffers();
		void Draw();
};