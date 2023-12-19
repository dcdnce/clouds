#pragma once

#include "pfm/pfm.hpp"

struct Vertex {
    pfm::vec3 position;
    pfm::vec3 normal;
    pfm::vec2 texCoords;
	pfm::vec3 color;
};
