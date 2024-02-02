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
		pfm::vec3 sun_position;
		float _radius;

	public:
		Shader shader;
		unsigned char texture[256*256];

		Skydome();
		~Skydome();
		void ComputePositions(float const& radius, size_t const& num_rows, size_t const& num_cols);
		void ComputeTexCoords();
		void SendBuffers();

		inline void Draw(int const frames, pfm::vec3 const camera_position)
		{
			glUseProgram(this->shader.program);

			pfm::mat4 rotated_sun_mat = pfm::rotate(
				pfm::mat4(1.f),
				static_cast<float>(frames) * pfm::radians(0.01),
				pfm::vec3(0.f, 0.f, 1.f)
			);
			glUniformMatrix4fv(glGetUniformLocation(shader.program, "uRotatedSun"), 1, GL_FALSE, &rotated_sun_mat);
			glUniform1i(glGetUniformLocation(shader.program, "uFrames"), frames);
			glUniform3f(glGetUniformLocation(shader.program, "uCameraPosition"), camera_position.x, camera_position.y, camera_position.z);
			glUniform1f(glGetUniformLocation(shader.program, "uSkydomeRadius"), _radius);

			glBindVertexArray(_VAO);

			glDrawArrays(GL_TRIANGLES, 0, _vertices.size());

			glUseProgram(0);
			glBindVertexArray(0);
		}
};
