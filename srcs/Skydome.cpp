#include "Skydome.hpp"
#include "pfm/pfm.hpp"

Skydome::Skydome()
{}

Skydome::~Skydome()
{}

void Skydome::fillBuffers(float const& radius, size_t const& numRows, size_t const& numCols)
{
	int topStripNumVertices = 3 * numCols;
	int regularStripNumVertices = 6 * numCols;
	//minus 1 because of top strip
	_numVertices = topStripNumVertices + ((numRows - 1) * regularStripNumVertices);

	_vertices = std::vector<Vertex>(_numVertices);

	float pitchIncrement = 90.f / static_cast<float>(numRows);
	float headingIncrement = 360.f / static_cast<float>(numCols);

	// float pitch = -90.f; //really ?
	size_t i = 0;

	// Top strip
	for (float heading = 0.f ; heading < 360.f ; heading += headingIncrement)
	{
		_vertices[i++].position = pfm::vec3(0.f, radius, 0.f); // apex
		_vertices[i++].position = pfm::sphericalToCartesian(radius, -90.f + pitchIncrement, heading);
		_vertices[i++].position = pfm::sphericalToCartesian(radius, -90.f + pitchIncrement, heading + headingIncrement);
	}

	//Regular strips
	for (float pitch = -90.f + pitchIncrement ; pitch < 0.f ; pitch += pitchIncrement)
	{
		for (float heading = 0.f ; heading < 360.f ; heading += headingIncrement)
		{
			pfm::vec3 v0 = pfm::sphericalToCartesian(radius, pitch, heading);
			pfm::vec3 v1 = pfm::sphericalToCartesian(radius, pitch, heading+headingIncrement);
			pfm::vec3 v2 = pfm::sphericalToCartesian(radius, pitch+pitchIncrement, heading);
			pfm::vec3 v3 = pfm::sphericalToCartesian(radius, pitch+pitchIncrement, heading+headingIncrement);

			assert(i + 6 <= _numVertices);

			_vertices[i++].position = v0;
			_vertices[i++].position = v1;
			_vertices[i++].position = v2;

			_vertices[i++].position = v1;
			_vertices[i++].position = v3;
			_vertices[i++].position = v2;
		}
	}

	for (size_t i = 0 ; i < _vertices.size() ; i++)
    {
        _vertices[i].color = {
            static_cast<float>(rand()%255) / 255.f,
            static_cast<float>(rand()%255) / 255.f,
            static_cast<float>(rand()%255) / 255.f
        };
    }
}

void Skydome::sendBuffers()
{
	glGenVertexArrays(1, &_VAO);
	glGenBuffers(1, &_VBO);
	glBindVertexArray(_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, _VBO);
	glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(Vertex), _vertices.data(), GL_STATIC_DRAW);

	// Link
	 /* Position */
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
        /* Normal */
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)(offsetof(Vertex, normal)));
        /* TexCoords */
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)(offsetof(Vertex, texCoords)));
        /* Color */
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)(offsetof(Vertex, color)));
	
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Skydome::draw()
{
	glUseProgram(this->shader.program);
	glBindVertexArray(_VAO);

	glDrawArrays(GL_TRIANGLES, 0, _vertices.size());

	glUseProgram(0);
	glBindVertexArray(0);
}