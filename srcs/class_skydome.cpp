#include "class_skydome.h"
#include "pfm/pfm.hpp"
#include <cmath>

Skydome::Skydome()
{}

Skydome::~Skydome()
{}

void Skydome::ComputePositions(float const& radius, size_t const& num_rows, size_t const& num_cols)
{
	int top_strip_num_vertices = 3 * num_cols;
	int regular_strip_num_vertices = 6 * num_cols;
	//minus 1 because of top strip
	_num_vertices = top_strip_num_vertices + ((num_rows - 1) * regular_strip_num_vertices);

	_vertices = std::vector<Vertex>(_num_vertices);

	float pitch_increment = 90.f / static_cast<float>(num_rows);
	float heading_increment = 360.f / static_cast<float>(num_cols);

	size_t i = 0;
	
	// Top strips
	for (float heading = 0.f ; heading < 360.f ; heading += heading_increment) {
		_vertices[i++].position = pfm::vec3(0.f, radius, 0.f);
		_vertices[i++].position = pfm::sphericalToCartesian(radius, -pitch_increment, heading + heading_increment);
		_vertices[i++].position = pfm::sphericalToCartesian(radius, -pitch_increment, heading);
	}

 	// Regular strips
	for (float pitch = -90.f; pitch < -pitch_increment ; pitch += pitch_increment) {
		for (float heading = 0.f ; heading < 360.f ; heading += heading_increment) {
			pfm::vec3 v0 = pfm::sphericalToCartesian(radius, pitch, heading);
			pfm::vec3 v1 = pfm::sphericalToCartesian(radius, pitch, heading+heading_increment);
			pfm::vec3 v2 = pfm::sphericalToCartesian(radius, pitch+pitch_increment, heading);
			pfm::vec3 v3 = pfm::sphericalToCartesian(radius, pitch+pitch_increment, heading+heading_increment);

			assert(i + 6 <= _num_vertices);

			_vertices[i++].position = v0;
			_vertices[i++].position = v1;
			_vertices[i++].position = v2;
			//// Normals
			//pfm::vec3 vn1 = v1 - v0;
			//pfm::vec3 vn2 = v2 - v0;
			//_vertices[i-2].normal = pfm::normalize(pfm::cross(vn1, vn2));
			//_vertices[i-1].normal = pfm::normalize(pfm::cross(vn1, vn2));
			//_vertices[i].normal = pfm::normalize(pfm::cross(vn1, vn2));

			_vertices[i++].position = v1;
			_vertices[i++].position = v3;
			_vertices[i++].position = v2;
			//// Normals
			//vn1 = v3 - v1;
			//vn2 = v2 - v1;
			//_vertices[i-2].normal = pfm::normalize(pfm::cross(vn1, vn2));
			//_vertices[i-1].normal = pfm::normalize(pfm::cross(vn1, vn2));
			//_vertices[i].normal = pfm::normalize(pfm::cross(vn1, vn2));

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
	for (size_t i = 0 ; i < _num_vertices ; i++) {
		pfm::vec3 d = pfm::normalize(pfm::vec3(0.f, 0.f, 0.f) - _vertices[i].position);
		_vertices[i].tex_coords.u = (atan2(d.z, d.x) / (2*M_PI)) + 0.5;
		_vertices[i].tex_coords.v = (asin(d.y) / M_PI) + 0.5;
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
