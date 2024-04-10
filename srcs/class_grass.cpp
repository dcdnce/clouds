#include "class_grass.h"


Grass::Grass(FastNoiseLite const & noise, pfm::vec3 const & cam)
{
	for (float i = -10; i < 10; i += 0.1) {
		for (float j = -10; j < 10; j += 0.1) {
			float noise_value = (noise.GetNoise(i + cam.x, j + cam.z) + 1.0f) * 0.5f;
			noise_value *= 50.f;
			std::max(noise_value, 10.f);

			Vertex curr;
			curr.position = pfm::vec3(i + cam.x, 6000.f + noise_value, j + cam.z);
			_vertices.push_back(curr);
		}
	}

	std::cout << _vertices.size() << std::endl;

	_SetupBuffers();
}

Grass::~Grass()
{}

void Grass::_SetupBuffers()
{
	glGenVertexArrays(1, &_VAO);
	glGenBuffers(1, &_VBO);
	glBindVertexArray(_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, _VBO);
	glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(Vertex), _vertices.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)(offsetof(Vertex, normal)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, tex_coords)));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, color)));

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}