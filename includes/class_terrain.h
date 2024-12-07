#pragma once
#include <vector>
#include "glad/glad.h"
#include "class_vertex.h"
#include "class_shader.h"
#include "class_engine.h"
#include "FastNoiseLite.h"

class Terrain {
	private:
		GLuint _VAO, _VBO, _EBO;
		std::vector<Vertex> _vertices;
		std::vector<GLuint> _indices;

	public:
		Terrain(size_t const size);
		~Terrain();
		Shader shader;
		FastNoiseLite noise;
		size_t noise_size;

		void SetupBuffers();
		pfm::vec3 WhichBiome(float const e);

		inline void Draw(int const frames, Engine& e, pfm::mat4 proj_sun = pfm::mat4(1.f), pfm::mat4 view_sun = pfm::mat4(1.f),
		                 float const zenith = 1.f, pfm::mat4 rotated_sun_mat = pfm::mat4(1.f))
		{
			glUseProgram(shader.program);
			glUniformMatrix4fv(glGetUniformLocation(shader.program, "uRotatedSun"), 1, GL_FALSE, &rotated_sun_mat);
			glUniformMatrix4fv(glGetUniformLocation(shader.program, "uProjSun"), 1, GL_FALSE, &proj_sun);
			glUniformMatrix4fv(glGetUniformLocation(shader.program, "uViewSun"), 1, GL_FALSE, &view_sun);
			glUniform3f(glGetUniformLocation(shader.program, "uSunPosition"), e.sun_position.x, e.sun_position.y, e.sun_position.z);
			glUniform1i(glGetUniformLocation(shader.program, "uFrames"), frames);
			glUniform3f(glGetUniformLocation(shader.program, "uCameraPosition"), e.camera.position.x, e.camera.position.y,
			            e.camera.position.z);
			glUniform1f(glGetUniformLocation(shader.program, "uZenithalOpticalLengthAir"), e.optical_length_air);
			glUniform1f(glGetUniformLocation(shader.program, "uZenithalOpticalLengthHaze"), e.optical_length_haze);
			glUniform1f(glGetUniformLocation(shader.program, "uZenith"), zenith);
			glUniform1f(glGetUniformLocation(shader.program, "uG"), e.mie_phase_eccentricity);


			glBindVertexArray(_VAO);
			glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
			glUseProgram(0);
		}
};