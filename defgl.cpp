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

bool DEPTH_TEST = false;

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

int setDepthTest(const bool &v)
{
	DEPTH_TEST = v;
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
		Vecf v0 = shader->vertex(data + bytes_per_vertex * indices[i * 3	]);
		Vecf v1 = shader->vertex(data + bytes_per_vertex * indices[i * 3 + 1]);
		Vecf v2 = shader->vertex(data + bytes_per_vertex * indices[i * 3 + 2]);

		bool swapped01;
		bool swapped02;
		bool swapped12;

		int x0 = v0.x*width;
		int y0 = v0.y*height;
		int x1 = v1.x*width;
		int y1 = v1.y*height;
		int x2 = v2.x*width;
		int y2 = v2.y*height;

		if (swapped01 = v0.y > v1.y)
		{
			std::swap(x0, x1);
			std::swap(y0, y1);
		}
		if (swapped02 = v0.y > v2.y)
		{
			std::swap(x0, x2);
			std::swap(y0, y2);
		}
		if (swapped12 = v1.y > v2.y)
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
				if (y < 0 || x < 0 || y >= width || x >= height)continue;

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

				Vecf p = Vecf(x - unswapped_x0, y - unswapped_y0, 0);
				Vecf a = Vecf(unswapped_x1 - unswapped_x0, unswapped_y1 - unswapped_y0, 0);
				Vecf b = Vecf(unswapped_x2 - unswapped_x0, unswapped_y2 - unswapped_y0, 0);

				Vecf pb = b - p;

				float f = 1.0f / ((a.x)*(b.y) - (a.y)*(b.x));
				u = (pb.y * p.x - pb.x * p.y)*f;
				v = 1.0f - (u + ((a.x - p.x)*pb.y - (a.y - p.y) * pb.x)*f);
				//std::cout << u << " " << v << std::endl;

				float depth = mix(mix(v0.z, v1.z, u), v2.z, v);

				if (DEPTH_TEST && depth < getDepthAt(x, y))continue;

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