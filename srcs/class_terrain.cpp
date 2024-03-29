#include "class_terrain.h"

Terrain::Terrain(size_t const size)
{
	_noise_size = size;
	_vertices = std::vector<Vertex>(size*size);

	_noise.SetSeed(1337);
	_noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);

	glGenBuffers(1, &_VBO);
	glGenBuffers(1, &_EBO);
	glGenVertexArrays(1, &_VAO);
}

void Terrain::SetupBuffers()
{
	float const sizebytwo = _noise_size / 2;

	size_t index = 0;
	for (float i = -sizebytwo ; i < sizebytwo ; i++) {
		for (float j = -sizebytwo ; j < sizebytwo ; j++) {
			float noise_value = (_noise.GetNoise(i, j) + 1.0f) * 0.5f;
			noise_value *= 50.f;
			_vertices[index].position = pfm::vec3(i, 6000.f + noise_value, j);
			index++;
		}
	}

	pfm::vec3 normal;
	for (size_t i = 0 ; i < (_noise_size*_noise_size) - _noise_size ; i++) {
		if ((i % _noise_size) == 0) continue ;
		normal = pfm::cross(_vertices[i+1].position - _vertices[i].position, _vertices[i+_noise_size].position - _vertices[i].position);
		_indices.push_back(i);
		_indices.push_back(i+1);
		_indices.push_back(i+_noise_size);
		_vertices[i].normal = normal;
		_vertices[i+1].normal = normal;
		_vertices[i+_noise_size].normal = normal;
		normal = pfm::cross(_vertices[i+_noise_size].position - _vertices[i+1].position, _vertices[i+_noise_size+1].position - _vertices[i+1].position);
		_indices.push_back(i+1);
		_indices.push_back(i+_noise_size);
		_indices.push_back(i+_noise_size+1);
		_vertices[i+1].normal = normal;
		_vertices[i+_noise_size].normal = normal;
		_vertices[i+_noise_size+1].normal = normal;
	}

	glBindVertexArray(_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, _VBO);
	glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(Vertex), _vertices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(GLuint), _indices.data(), GL_STATIC_DRAW);

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
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}


Terrain::~Terrain() {}