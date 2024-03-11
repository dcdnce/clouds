#pragma once

#include <iostream>
#include "glad/glad.h"
#include <vector>
#include "class_vertex.h"
#include "class_shader.h"
#include "class_engine.h"

class Skydome {
	private:
		GLuint _VBO;
		GLuint _VAO;
		size_t _num_vertices;
		std::vector<Vertex> _vertices;
		float _radius;

	public:
		Shader shader;
		unsigned char texture[256*256];

		Skydome();
		~Skydome();
		void ComputePositions(float const& radius, size_t const& num_rows, size_t const& num_cols);
		void ComputeTexCoords();
		void SendBuffers();

		inline void Draw(int const frames, Engine & e)
		{
			glUseProgram(this->shader.program);

			pfm::mat4 rotated_sun_mat = pfm::rotate(pfm::mat4(1.f), static_cast<float>(frames) * pfm::radians(0.01), pfm::vec3(0.f, 0.f, 1.f));
			float zenith = static_cast<float>(pfm::magnitude(e.camera.position - pfm::vec3(0.f, _radius, 0.f)));
			glUniformMatrix4fv(glGetUniformLocation(shader.program, "uRotatedSun"), 1, GL_FALSE, &rotated_sun_mat);
			glUniform1i(glGetUniformLocation(shader.program, "uFrames"), frames);
			glUniform3f(glGetUniformLocation(shader.program, "uCameraPosition"), e.camera.position.x, e.camera.position.y, e.camera.position.z);
			glUniform3f(glGetUniformLocation(shader.program, "uSunPosition"), e.sun_position.x, e.sun_position.y, e.sun_position.z);
			glUniform1f(glGetUniformLocation(shader.program, "uZenith"), zenith);
			glUniform1i(glGetUniformLocation(shader.program, "uCloudsRender"), e.clouds_render);
			glUniform1f(glGetUniformLocation(shader.program, "uCloudsSmoothstepEdgeMin"), e.clouds_smoothstep_edge_min);
			glUniform1f(glGetUniformLocation(shader.program, "uCloudsSmoothstepEdgeMax"), e.clouds_smoothstep_edge_max);
			glUniform1i(glGetUniformLocation(shader.program, "uAverageDensity"), e.average_density);
			glUniform1f(glGetUniformLocation(shader.program, "uAverageDensityStepSize"), e.average_density_step_size);
			glUniform1f(glGetUniformLocation(shader.program, "uNoiseScale"), e.noise_scale);
			glUniform1f(glGetUniformLocation(shader.program, "uOpticalLengthAir"), e.optical_length_air);
			glUniform1f(glGetUniformLocation(shader.program, "uOpticalLengthHaze"), e.optical_length_haze);

			glBindVertexArray(_VAO);
			glDrawArrays(GL_TRIANGLES, 0, _vertices.size());

			glUseProgram(0);
			glBindVertexArray(0);
		}

		inline void DrawWith(int const frames, Engine & e, Shader & shadow_map_shader)
		{
			glUseProgram(shadow_map_shader.program);
			pfm::mat4 rotated_sun_mat = pfm::rotate(pfm::mat4(1.f), static_cast<float>(frames) * pfm::radians(0.01), pfm::vec3(0.f, 0.f, 1.f));

			glUniformMatrix4fv(glGetUniformLocation(shadow_map_shader.program, "uRotatedSun"), 1, GL_FALSE, &rotated_sun_mat);
			glUniform3f(glGetUniformLocation(shadow_map_shader.program, "uSunPosition"), e.sun_position.x, e.sun_position.y, e.sun_position.z);
			glUniform1i(glGetUniformLocation(shadow_map_shader.program, "uFrames"), frames);
			glUniform1f(glGetUniformLocation(shadow_map_shader.program, "uCloudsSmoothstepEdgeMin"), e.clouds_smoothstep_edge_min);
			glUniform1f(glGetUniformLocation(shadow_map_shader.program, "uCloudsSmoothstepEdgeMax"), e.clouds_smoothstep_edge_max);
			glUniform1i(glGetUniformLocation(shadow_map_shader.program, "uAverageDensity"), e.average_density);
			glUniform1f(glGetUniformLocation(shadow_map_shader.program, "uAverageDensityStepSize"), e.average_density_step_size);
			glUniform1f(glGetUniformLocation(shadow_map_shader.program, "uNoiseScale"), e.noise_scale);

			glBindVertexArray(_VAO);
			glDrawArrays(GL_TRIANGLES, 0, _vertices.size());

			glUseProgram(0);
			glBindVertexArray(0);
		}
};
