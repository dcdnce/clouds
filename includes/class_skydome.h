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
		size_t _numVertices;
		std::vector<Vertex> _vertices;
	public:
		Shader shader;	 

		Skydome();
		~Skydome();
		void fillBuffers(float const& radius, size_t const& numRows, size_t const& numCols);
		void sendBuffers();
		void draw();
};