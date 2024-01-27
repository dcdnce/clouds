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
		unsigned char noise[8][256*256];
		GLuint texture_id;
		size_t _num_vertices;
		std::vector<Vertex> _vertices;


		void _NoiseComposition(int const frames);
	public:
		Shader shader;
		unsigned char texture[256*256];

		Skydome();
		~Skydome();
		void ComputePositions(float const& radius, size_t const& num_rows, size_t const& num_cols);
		void ComputeTexCoords();
		void SendBuffers();
		void CreateTexture();
		void NoiseInterpolation(int const seed);

		inline void Draw(int const frames)
		{
			glUseProgram(this->shader.program);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture_id);
			_NoiseComposition(frames);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, 256, 256, 0, GL_RED, GL_UNSIGNED_BYTE, texture);

			glBindVertexArray(_VAO);

			glDrawArrays(GL_TRIANGLES, 0, _vertices.size());

			glUseProgram(0);
			glBindVertexArray(0);
		}
};
