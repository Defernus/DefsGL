#pragma once

/**
 *DefsGL graphic library
 *
 *
 *
 *made by Defernus for fun
 */

#include "vec.h"

struct Color
{
	float r, g, b, a;
};

uint32_t colorToInt(const Color&);

class Shader
{
public:
	virtual Vec4f vertex(const unsigned char *data) = 0;
	virtual Color fragment(const float &u, const float &v) = 0;
};

int initDGL();
int bindShader(Shader*);
int bindBitmap(unsigned char*, const size_t &width, const size_t &height);
int clearColor(Color color);
int clearDepthBuffer(const float&);

int setDepthTestFunction(bool(*depthTestFunction)(const float &input, const float&current));

int drawElements(const unsigned char *data, const uint32_t *indices, const size_t &quantity, const size_t &bytes_per_vertex);

int terminateDGL();