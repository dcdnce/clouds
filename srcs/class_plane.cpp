#include "class_plane.h"

Plane::Plane()
{
	const float y = 5999.f;
	Vertex v;
	v.position = pfm::vec3(-5000.f, y, -5000.f);
	v.color = pfm::vec3(0.8f, 1.f, 0.8f);
	_vertices.push_back(v);
	v.position = pfm::vec3(5000.f, y, -5000.f);
	_vertices.push_back(v);
	v.position = pfm::vec3(-5000.f, y, 5000.f);
	_vertices.push_back(v);
	v.position = pfm::vec3(5000.f, y, 5000.f);
	_vertices.push_back(v);
	_indices.push_back(0);
	_indices.push_back(1);
	_indices.push_back(2);
	_indices.push_back(1);
	_indices.push_back(2);
	_indices.push_back(3);

	_SetupBuffers();
}

Plane::~Plane()
{}

void Plane::_SetupBuffers()
{
	glGenVertexArrays(1, &_VAO);
	glGenBuffers(1, &_VBO);
	glGenBuffers(1, &_EBO);
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

void Plane::Draw()
{
	glBindVertexArray(_VAO);
	glUseProgram(shader.program);
	glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glUseProgram(0);
}