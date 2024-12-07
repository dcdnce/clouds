#include "class_terrain.h"
#define FOREST	(pfm::vec3){86, 106, 61}
#define	ROCK	(pfm::vec3){185,156,150}
#define	SNOW (pfm::vec3){239,240,241}

Terrain::Terrain(size_t const size)
{
	noise_size = size;
	_vertices = std::vector<Vertex>(size * size);

	noise.SetSeed(1337);
	noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
	noise.SetFrequency(0.009);
	noise.SetFractalType(FastNoiseLite::FractalType_FBm);
	noise.SetFractalOctaves(10);
	noise.SetFractalLacunarity(2);
	noise.SetFractalGain(0.5f);
	noise.SetFractalWeightedStrength(0.1f);

	glGenBuffers(1, &_VBO);
	glGenBuffers(1, &_EBO);
	glGenVertexArrays(1, &_VAO);
}

pfm::vec3 Terrain::WhichBiome(float const e)
{
	if (e <= 0.3f) {
		return (FOREST);
	}
	if (e < 0.5f) {
		return (ROCK);
	}
	return (SNOW);
}


void Terrain::SetupBuffers()
{
	float const sizebytwo = noise_size / 2;

	size_t index = 0;
	for (float i = -sizebytwo ; i < sizebytwo ; i++) {
		for (float j = -sizebytwo ; j < sizebytwo ; j++) {
			float noise_value = (noise.GetNoise(i, j) + 1.0f) * 0.5f;
			noise_value = pow(noise_value * 1.2f, 1.5f);
			noise_value = std::max(noise_value, 0.3f);
			_vertices[index].color = WhichBiome(noise_value);
			noise_value *= 50.f;
			_vertices[index].position = pfm::vec3(i, 6000.f + noise_value, j);
			index++;
		}
	}

	// Normal et indices
	for (size_t i = 0; i < noise_size - 1; ++i) {
		for (size_t j = 0; j < noise_size - 1; ++j) {
			size_t index = i * noise_size + j;
			pfm::vec3 normal = pfm::normalize(pfm::cross(_vertices[index + 1].position - _vertices[index].position,
			                                  _vertices[index + noise_size].position - _vertices[index].position));
			_indices.push_back(index);
			_indices.push_back(index + noise_size);
			_indices.push_back(index + 1);
			_vertices[index].normal = normal;
			_vertices[index + 1].normal = normal;
			_vertices[index + noise_size].normal = normal;
			normal = pfm::normalize(pfm::cross(_vertices[index + noise_size].position - _vertices[index + 1].position,
			                                   _vertices[index + noise_size + 1].position - _vertices[index + 1].position));
			_indices.push_back(index + 1);
			_indices.push_back(index + noise_size + 1);
			_indices.push_back(index + noise_size);
			_vertices[index + 1].normal = normal;
			_vertices[index + noise_size].normal = normal;
			_vertices[index + noise_size + 1].normal = normal;
		}
	}

	glBindVertexArray(_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, _VBO);
	glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(Vertex), _vertices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(GLuint), _indices.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, normal)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, tex_coords)));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, color)));

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

Terrain::~Terrain() {}