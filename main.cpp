#include "vec.h"
#include "defgl.h"
#include "bmp.h"

class MainShader: public Shader
{
public:
	char i = 0;

	Vecf color[3];

	Vecf vertex(const unsigned char *data)
	{
		Vecf v;
		char j = 0;
		v.x = readFromBytes<float>(data + (sizeof(float) * (j++)));
		v.y = readFromBytes<float>(data + (sizeof(float) * (j++)));
		v.z = readFromBytes<float>(data + (sizeof(float) * (j++)));


		color[i].x = readFromBytes<float>(data + (sizeof(float) * (j++)));
		color[i].y = readFromBytes<float>(data + (sizeof(float) * (j++)));
		color[i].z = readFromBytes<float>(data + (sizeof(float) * (j++)));

		std::cout << int(i) << "\t\t" << v << "\t\t" << color[i] << std::endl;

		if (++i == 3) {
			i = 0;
		}


		return v;
	}

	Color fragment(const float &u, const float &v)
	{
		if(i != 0) std::cout << "wtf, i != 0 int fragment!!!" << std::endl;

		Vecf c = mix2d(color[0], color[1], color[2], u, v);

		//std::cout << u << "\t\t" << v << std::endl;
		//std::cout << c << std::endl;
		//std::cout << color[0] << "\t\t" << color[1] << "\t\t" << color[2] << std::endl << std::endl;

		return { c.x, c.y, c.z, 1 };
	}
};

int main()
{
	BMP image = { 512, 512 };

	size_t image_size = image.widht * image.height;

	image.bit_map = new unsigned char[image_size*3];

	float triangles[] =
	{
		.1, .1, .1,		1, 0, 0,
		.9, .1, .1,		0, 1, 0,
		.1, .9, .1,		0, 0, 1,
		.9, .9, .1,		1, 1, 1,
	};

	uint32_t indices[] =
	{
		0, 1, 2,
		1, 3, 2,
	};

	MainShader *shader = new MainShader();

	initDGL();
	bindShader(shader);
	bindBitmap(image.bit_map, image.widht, image.height);

	setDepthTest(true);

	clearColor({1, 0, 1, 1});
	clearDepthBuffer(-INFINITY);

	drawElements((unsigned char*)triangles, indices, 6, 6*sizeof(float));

	write(image, "test.bmp");

	std::cout << "success!" << std::endl;

	getchar();

	bmp_terminate(image);
	terminateDGL();

	return 0;
}