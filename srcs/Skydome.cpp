#include "Skydome.hpp"
#include "pfm/pfm.hpp"
#include <vector>
#include "Vertex.hpp"

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

	std::vector<Vertex> vertices(_numVertices);

	float pitchIncrement = 90.f / static_cast<float>(numRows);
	float headingIncrement = 360.f / static_cast<float>(numCols);

	// float pitch = -90.f; //really ?
	size_t i = 0;

	// Top strip
	for (float heading = 0.f ; heading < 360.f ; heading += headingIncrement)
	{
		vertices[i++].position = pfm::vec3(0.f, radius, 0.f); // apex
		vertices[i++].position = pfm::sphericalToCartesian(radius, -90.f + pitchIncrement, heading);
		vertices[i++].position = pfm::sphericalToCartesian(radius, -90.f + pitchIncrement, heading + headingIncrement);
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

			vertices[i++].position = v0;
			vertices[i++].position = v1;
			vertices[i++].position = v2;

			vertices[i++].position = v1;
			vertices[i++].position = v3;
			vertices[i++].position = v2;
		}
	}
}