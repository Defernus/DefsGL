#include "vec.h"
#include "defgl.h"
#include "bmp.h"
#include "model.h"

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

		Vecf uv;
		uv.x = readFromBytes<float>(data + (sizeof(float) * (j++)));
		uv.y = readFromBytes<float>(data + (sizeof(float) * (j++)));

		Vecf normal;
		normal.x = readFromBytes<float>(data + (sizeof(float) * (j++)));
		normal.y = readFromBytes<float>(data + (sizeof(float) * (j++)));
		normal.z = readFromBytes<float>(data + (sizeof(float) * (j++)));

		color[i] = normal;

		if (++i == 3) {
			i = 0;
		}


		return v;
	}

	Color fragment(const float &u, const float &v)
	{
		if(i != 0) std::cout << "wtf, i != 0 int fragment!!!" << std::endl;

		Vecf c = mix2d(color[0], color[1], color[2], u, v);

		return { c.x, c.y, c.z, 1 };
	}
};

int main()
{
	BMP image = { 512, 512 };

	size_t image_size = image.widht * image.height;

	image.bit_map = new unsigned char[image_size*3];

	Model model = loadModel("model.obj");

	MainShader *shader = new MainShader();

	initDGL();
	bindShader(shader);
	bindBitmap(image.bit_map, image.widht, image.height);

	setDepthTest(true);

	clearColor({.5, .5, .5, 1});
	clearDepthBuffer(-INFINITY);

	drawElements((unsigned char*)(&model.data[0]), (uint32_t*)(&model.indices[0]), model.indices.size(), 8*sizeof(float));

	writeBMP(image, "test.bmp");

	std::cout << "success!" << std::endl;

	getchar();

	bmp_terminate(image);
	terminateDGL();

	return 0;
}