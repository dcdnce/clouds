#pragma once
#include <vector>
#include "glad/glad.h"
#include "class_vertex.h"
#include "class_shader.h"

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
		void Draw();
};