#include "class_terrain.h"

Terrain::Terrain(size_t const size)
{
	noise_size = size;
	_vertices = std::vector<Vertex>(size*size);

	noise.SetSeed(1337);
	noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
	noise.SetFrequency(0.019);
	noise.SetFractalType(FastNoiseLite::FractalType_FBm);
	noise.SetFractalOctaves(5);
	noise.SetFractalLacunarity(2);
	noise.SetFractalGain(0.5f);
	noise.SetFractalWeightedStrength(0.1f);

	glGenBuffers(1, &_VBO);
	glGenBuffers(1, &_EBO);
	glGenVertexArrays(1, &_VAO);
}

void Terrain::SetupBuffers()
{
	float const sizebytwo = noise_size / 2;

	size_t index = 0;
	for (float i = -sizebytwo ; i < sizebytwo ; i++) {
		for (float j = -sizebytwo ; j < sizebytwo ; j++) {
			float noise_value = (noise.GetNoise(i, j) + 1.0f) * 0.5f;
			noise_value = std::max(noise_value, 0.5f);
			noise_value *= 50.f;
			_vertices[index].position = pfm::vec3(i, 6000.f + noise_value, j);
			index++;
		}
	}

	// Normal et indices
	for (size_t i = 0; i < noise_size - 1; ++i) {
		for (size_t j = 0; j < noise_size - 1; ++j) {
			size_t index = i * noise_size + j;
			pfm::vec3 normal = pfm::normalize(pfm::cross(_vertices[index+1].position - _vertices[index].position, _vertices[index+noise_size].position - _vertices[index].position));
			_indices.push_back(index);
			_indices.push_back(index+noise_size);
			_indices.push_back(index+1);
			_vertices[index].normal = normal;
			_vertices[index+1].normal = normal;
			_vertices[index+noise_size].normal = normal;
			normal = pfm::normalize(pfm::cross(_vertices[index+noise_size].position - _vertices[index+1].position, _vertices[index+noise_size+1].position - _vertices[index+1].position));
			_indices.push_back(index+1);
			_indices.push_back(index+noise_size+1);
			_indices.push_back(index+noise_size);
			_vertices[index+1].normal = normal;
			_vertices[index+noise_size].normal = normal;
			_vertices[index+noise_size+1].normal = normal;
		}
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

void Terrain::InitDepthMap()
{
	glGenFramebuffers(1, &depth_map_FBO);

	// Texture
	glGenTextures(1, &depth_map_texture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, depth_map_texture);
	// glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1024, 1024, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL); // Color
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 4024, 4024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);

	// Attach texture to framebuffer as depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, depth_map_FBO);
	// glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, depth_map, 0); // Color
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth_map_texture, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Shader
	depth_map_shader.LoadShaders("./shaders/terrain_depth_map.vert", "./shaders/terrain_depth_map.frag");
}

void Terrain::DrawDepthMap(int frames, Engine & e)
{
	glViewport(0, 0, 4024, 4024);
	glBindFramebuffer(GL_FRAMEBUFFER, depth_map_FBO); // drawing to	
	glClearColor(0.f, 0.2f, 1.f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_ALWAYS);
	glEnable(GL_DEPTH_TEST);

	pfm::mat4 rotated_sun_mat = pfm::rotate(pfm::mat4(1.f), static_cast<float>(frames) * pfm::radians(0.001), pfm::vec3(0.f, 0.f, 1.f));
	pfm::vec4 sp = rotated_sun_mat * pfm::vec4(e.sun_position.x, e.sun_position.y, e.sun_position.z, 1.f);
	depth_map_shader.SetViewMat(pfm::lookAt(pfm::vec3(sp.x, sp.y, sp.z), pfm::vec3(0.f, 6000.f, 0.f), pfm::vec3(1.f, 0.f, 0.f)));
	depth_map_shader.SetModelMat(pfm::mat4(1.f));
	glUseProgram(depth_map_shader.program);
	glBindVertexArray(_VAO);
	glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glUseProgram(0);

	glDepthFunc(GL_LESS);
}


Terrain::~Terrain() {}