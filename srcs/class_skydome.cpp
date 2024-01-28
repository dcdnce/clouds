#include "class_skydome.h"
#include "pfm/pfm.hpp"
#include <cmath>

Skydome::Skydome()
{}

Skydome::~Skydome()
{}

void Skydome::ComputePositions(float const& radius, size_t const& num_rows, size_t const& num_cols)
{
	int top_strip_num_vertices = 6 * num_cols;
	int regular_strip_num_vertices = 6 * num_cols;
	//minus 1 because of top strip
	_num_vertices = top_strip_num_vertices + ((num_rows - 1) * regular_strip_num_vertices);

	_vertices = std::vector<Vertex>(_num_vertices);

	float heading_increment = 360.f / static_cast<float>(num_cols);
	float pitch_increment = 90.f / static_cast<float>(num_rows);
	float u_increment = heading_increment / 360.f;
	float v_increment = pitch_increment / 180.f;

	size_t i = 0;
	
	// Top strips
	for (float heading = 0.f, u = 0.f ; heading < 360.f ; heading += heading_increment, u += u_increment) {
		pfm::vec3 v0(0.f, radius, 0.f); 
		pfm::vec2 v0_tex_coords(u - (u/2.f), 1.f);
		pfm::vec3 v1(0.f, radius, 0.f); 
		pfm::vec2 v1_tex_coords(u, 1.f);

		pfm::vec3 v2 = pfm::sphericalToCartesian(radius, -pitch_increment, heading);
		pfm::vec2 v2_tex_coords(u, 1.f - v_increment);
		pfm::vec3 v3 = pfm::sphericalToCartesian(radius, -pitch_increment, heading+heading_increment);
		pfm::vec2 v3_tex_coords(u + u_increment, 1.f - v_increment);

		_vertices[i].tex_coords = v0_tex_coords;
		_vertices[i++].position = v0;
		_vertices[i].tex_coords = v1_tex_coords;
		_vertices[i++].position = v1;
		_vertices[i].tex_coords = v2_tex_coords;
		_vertices[i++].position = v2;

		_vertices[i].tex_coords = v1_tex_coords;
		_vertices[i++].position = v1;
		_vertices[i].tex_coords = v3_tex_coords;
		_vertices[i++].position = v3;
		_vertices[i].tex_coords = v2_tex_coords;
		_vertices[i++].position = v2;
	}

 	// Regular strips
	for (float pitch = -90.f, v = 0.f; pitch < -pitch_increment ; pitch += pitch_increment, v += v_increment) {
		for (float heading = 0.f, u = 0.f ; heading < 360.f ; heading += heading_increment, u += u_increment) {
			pfm::vec3 v0 = pfm::sphericalToCartesian(radius, pitch, heading);
			pfm::vec2 v0_tex_coords(u, v);
			pfm::vec3 v1 = pfm::sphericalToCartesian(radius, pitch, heading+heading_increment);
			pfm::vec2 v1_tex_coords(u + u_increment, v);
			pfm::vec3 v2 = pfm::sphericalToCartesian(radius, pitch+pitch_increment, heading);
			pfm::vec2 v2_tex_coords(u, v + v_increment);
			pfm::vec3 v3 = pfm::sphericalToCartesian(radius, pitch+pitch_increment, heading+heading_increment);
			pfm::vec2 v3_tex_coords(u + u_increment, v + v_increment);

			assert(i + 6 <= _num_vertices);

			_vertices[i].tex_coords = v0_tex_coords;
			_vertices[i++].position = v0;
			_vertices[i].tex_coords = v1_tex_coords;
			_vertices[i++].position = v1;
			_vertices[i].tex_coords = v2_tex_coords;
			_vertices[i++].position = v2;

			_vertices[i].tex_coords = v1_tex_coords;
			_vertices[i++].position = v1;
			_vertices[i].tex_coords = v3_tex_coords;
			_vertices[i++].position = v3;
			_vertices[i].tex_coords = v2_tex_coords;
			_vertices[i++].position = v2;
		}
	}

	for (size_t i = 0 ; i < _vertices.size() ; i++) {
		_vertices[i].color = {
			static_cast<float>(rand()%255) / 255.f,
			static_cast<float>(rand()%255) / 255.f,
			static_cast<float>(rand()%255) / 255.f
		};
	}
}

void Skydome::ComputeTexCoords()
{
	float max_u = 0.f;
	float min_u = 0.f;
	float max_v = 0.f;
	float min_v = 0.f;

	for (size_t i = 0 ; i < _num_vertices ; i++) {
		pfm::vec3 v = _vertices[i].position;
		float kR = 10.f;

		_vertices[i].tex_coords.u = kR * tan(v.x / kR);
		_vertices[i].tex_coords.v = kR * tan(v.z / kR);

		max_u = std::max(max_u, _vertices[i].tex_coords.u);
		max_v = std::max(max_v, _vertices[i].tex_coords.v);
		min_u = std::min(min_u, _vertices[i].tex_coords.u);
		min_v = std::min(min_v, _vertices[i].tex_coords.v);
	}

	max_u += std::abs(min_u);
	max_v += std::abs(min_v);

	for (size_t i = 0 ; i < _num_vertices ; i++) {
		_vertices[i].tex_coords.u += std::abs(min_u);
		_vertices[i].tex_coords.v += std::abs(min_v);

		_vertices[i].tex_coords.u /= max_u;
		_vertices[i].tex_coords.v /= max_v;
	}
}

void Skydome::SendBuffers()
{
	glGenVertexArrays(1, &_VAO);
	glGenBuffers(1, &_VBO);
	glBindVertexArray(_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, _VBO);
	glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(Vertex), _vertices.data(), GL_STATIC_DRAW);

	// Link
	/* Position */
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	/* Normal */
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, normal)));
	/* tex_coords */
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, tex_coords)));
	/* Color */
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, color)));

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Skydome::CreateTexture()
{
	glUseProgram(shader.program);
	glGenTextures(1, &texture_id);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(glGetUniformLocation(shader.program, "texture1"), 0);
	glUseProgram(0);
}
