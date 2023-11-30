#pragma once

#include <iostream>

class Skydome {
	private:
		size_t _numVertices;
	public:
		Skydome();
		~Skydome();

		void fillBuffers(float const& radius, size_t const& numRows, size_t const& numCols);
};