#pragma once

#include "vec.h"
#include <vector>

struct Model
{
	std::vector<float> data;
	std::vector<uint32_t> indices;
};

Model loadModel(const char *path);