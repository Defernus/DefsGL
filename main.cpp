#include "vec.h"

#include "defgl.h"
#include "bmp.h"
#include "model.h"

class MainShader: public Shader
{
public:
	char i = 0;

	

	Vec3f color[3];

	Vec3f vertex(const unsigned char *data)
	{
		Vec3f v;
		char j = 0;
		v[0] = readFromBytes<float>(data + (sizeof(float) * (j++)));
		v[1] = readFromBytes<float>(data + (sizeof(float) * (j++)));
		v[2] = readFromBytes<float>(data + (sizeof(float) * (j++)));

		Vec2f uv;
		uv[0] = readFromBytes<float>(data + (sizeof(float) * (j++)));
		uv[1] = readFromBytes<float>(data + (sizeof(float) * (j++)));

		Vec3f normal;
		normal[0] = readFromBytes<float>(data + (sizeof(float) * (j++)));
		normal[1] = readFromBytes<float>(data + (sizeof(float) * (j++)));
		normal[2] = readFromBytes<float>(data + (sizeof(float) * (j++)));

		color[i] = normal;
		/*
		std::cout << int(i) << std::endl;
		std::cout << v << std::endl;
		std::cout << uv << std::endl;
		std::cout << normal << std::endl;
		std::cout << std::endl;
		std::cout << std::endl;
		*/
		if (++i == 3) {
			i = 0;
		}


		return v;
	}

	Color fragment(const float &u, const float &v)
	{
		if(i != 0) std::cout << "wtf, i != 0 int fragment!!!" << std::endl;

		Vec3f c = mix2d(color[0], color[1], color[2], u, v);

		return { c[0], c[1], c[2], 1 };
	}
};

int main()
{
	BMP image = { 512, 512 };

	size_t image_size = image.widht * image.height;

	image.bit_map = new unsigned char[image_size*3];

	Model model = loadModel("res/model.obj");

	MainShader *shader = new MainShader();

	initDGL();
	bindShader(shader);
	bindBitmap(image.bit_map, image.widht, image.height);

	setDepthTestFunction(([](const float &input, const float&current) {return input > current; }));

	clearColor({.5, .5, .5, 1});
	clearDepthBuffer(-INFINITY);

	drawElements((unsigned char*)(&model.data[0]), (uint32_t*)(&model.indices[0]), model.indices.size(), 8*sizeof(float));

	writeBMP(image, "out/test.bmp");

	std::cout << "success!" << std::endl;
	getchar();

	bmp_terminate(image);
	terminateDGL();

	return 0;
}