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
		Grass();
		~Grass();
		inline void Draw(int const frames)
		{
			glUseProgram(shader.program);
			glUniform1i(glGetUniformLocation(shader.program, "uFrames"), frames);
			glBindVertexArray(_VAO);
			glDrawArrays(GL_POINTS, 0, _vertices.size());
			glBindVertexArray(0);
			glUseProgram(0);
		}
};