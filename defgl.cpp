#include "defgl.h"

inline unsigned char getByteColor(const double &a)
{
	return unsigned char((a < 0 ? 0 : (a > 1 ? 1 : a)) * 255 + .5);
}


uint32_t colorToInt(const Color& color)
{
	union
	{
		unsigned char b[4];
		uint32_t i;
	}v;

	v.b[3] = clearDepthBuffer(color.r);
	v.b[2] = clearDepthBuffer(color.g);
	v.b[1] = clearDepthBuffer(color.b);
	v.b[0] = clearDepthBuffer(color.a);

	return v.i;
}

Shader *shader;
unsigned char *bitmap;
size_t bitmap_length;
size_t image_size;
size_t width;
size_t height;
float *depthBuffer;

bool(*depthTestFunction)(const float &input, const float&current) = ([](const float &input, const float&current) {return true; });

int initDGL()
{

	return 0;
}

int bindShader(Shader* _shader)
{
	shader = _shader;
	return 0;
}

int bindBitmap(unsigned char* bm, const size_t &w, const size_t &h)
{
	bitmap = bm;
	width = w;
	height = h;
	image_size = w * h;
	bitmap_length = image_size * 3;

	delete[] depthBuffer;
	depthBuffer = new float[image_size];
	return 0;
}

int clearColor(Color color)
{
	unsigned x = 0;
	unsigned y = 0;
	
	for (unsigned char *i = bitmap; i != bitmap + bitmap_length; i += 3)
	{
		if (x++ == width)
		{
			x = 0;
			++y;
			if (y == height) y = 0;
		}
		i[2] = getByteColor(color.r);
		i[1] = getByteColor(color.g);
		i[0] = getByteColor(color.b);
	}
	return 0;
}

int clearDepthBuffer(const float &a)
{
	for (float *i = depthBuffer; i != depthBuffer + image_size; ++i)i[0] = a;
	return 0;
}

inline void putPixel(const int &x, const int &y, const Color &color)
{
	uint32_t pos = (x + y * width)*3;

	bitmap[pos+2] = getByteColor(color.r);
	bitmap[pos+1] = getByteColor(color.g);
	bitmap[pos  ] = getByteColor(color.b);
}

int setDepthTestFunction(bool(*f)(const float &input, const float&current))
{
	depthTestFunction = f;
	return 0;
}

inline float &getDepthAt(const int &x, const int &y)
{
	return *(depthBuffer + (x+y*width));
}

int drawElements(const unsigned char *data, const uint32_t *indices, const size_t &quantity, const size_t &bytes_per_vertex)
{
	size_t triangles_amount = quantity/3;
	for (uint32_t i = 0; i != triangles_amount; ++i)
	{
		Vec4f v0 = shader->vertex(data + bytes_per_vertex * indices[i * 3	]);
		Vec4f v1 = shader->vertex(data + bytes_per_vertex * indices[i * 3 + 1]);
		Vec4f v2 = shader->vertex(data + bytes_per_vertex * indices[i * 3 + 2]);

		bool swapped01;
		bool swapped02;
		bool swapped12;

		int x0 = (v0[0] + 1) * (width / 2);
		int y0 = (v0[1] + 1) * (height / 2);
		int x1 = (v1[0] + 1) * (width / 2);
		int y1 = (v1[1] + 1) * (height / 2);
		int x2 = (v2[0] + 1) * (width / 2);
		int y2 = (v2[1] + 1) * (height / 2);

		if (swapped01 = y0 > y1)
		{
			std::swap(x0, x1);
			std::swap(y0, y1);
		}
		if (swapped02 = y0 > y2)
		{
			std::swap(x0, x2);
			std::swap(y0, y2);
		}
		if (swapped12 = y1 > y2)
		{
			std::swap(x1, x2);
			std::swap(y1, y2);
		}

		int h = y2 - y0;
		int h0 = y1 - y0;
		int h1 = y2 - y1;
		int w0 = x1 - x0;
		int w1 = x2 - x1;

		for (int y = y0; y != y2; ++y)
		{
			int xl = h > 0 ? mix((float)x0, (float)x2, (y - y0) / (double)h) : (x0<x2?x0:x2);
			int xr = (y-y0) < h0 ? mix(x0, x1, ((float)y - (float)y0) / (double)h0) : (h1 > 0 ? mix((float)x1, (float)x2, (y - y1) / (double)h1) : (x1>x2?x2:x1));

			bool lr_swapped;
			if (lr_swapped = xl > xr)
			{
				std::swap(xl, xr);
			}

			for (int x = xl; x != xr; ++x)
			{
				if (y < 0 || x < 0 || y >= int(width) || x >= int(height))continue;

				float u, v;

				int unswapped_x0 = x0;
				int unswapped_y0 = y0;
				int unswapped_x1 = x1;
				int unswapped_y1 = y1;
				int unswapped_x2 = x2;
				int unswapped_y2 = y2;

				if (swapped12)
				{
					std::swap(unswapped_x1, unswapped_x2);
					std::swap(unswapped_y1, unswapped_y2);
				}

				if (swapped02)
				{
					std::swap(unswapped_x0, unswapped_x2);
					std::swap(unswapped_y0, unswapped_y2);
				}

				if (swapped01)
				{
					std::swap(unswapped_x0, unswapped_x1);
					std::swap(unswapped_y0, unswapped_y1);
				}

				Vec2f p = Vec2f(x - unswapped_x0, y - unswapped_y0);
				Vec2f a = Vec2f(unswapped_x1 - unswapped_x0, unswapped_y1 - unswapped_y0);
				Vec2f b = Vec2f(unswapped_x2 - unswapped_x0, unswapped_y2 - unswapped_y0);

				Vec2f pb = b - p;

				float f = 1.0f / (a[0] * b[1] - a[1] * b[0]);
				u = (pb[1] * p[0] - pb[0] * p[1])*f;
				v = 1.0f - (u + ((a[0] - p[0])* pb[1] - (a[1] - p[1]) * pb[0])*f);
				//std::cout << u << " " << v << std::endl;

				float depth = mix2d(v0[2], v1[2], v2[2], u, v);

				if (depthTestFunction(getDepthAt(x, y), depth))continue;

				getDepthAt(x, y) = depth;

				putPixel(x, y, shader->fragment(u, v));
			}
		}
	}

	return 0;
}



int terminateDGL()
{
	delete[] depthBuffer;
	return 0;
}