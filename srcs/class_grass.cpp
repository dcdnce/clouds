#include "class_grass.h"

Grass::Grass()
{
	pfm::vec3 p = pfm::vec3(-50.f, 6000.f, -50.f);
	for (; p.x < 50.f; p.x += 10.f) {
		for (p.z = -50.f ; p.z < 50.f; p.z += 10.f) {
			Vertex curr;
			curr.position = p;
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