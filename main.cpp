#include "vec.h"

#include "defgl.h"
#include "bmp.h"
#include "model.h"

class MainShader: public Shader
{
public:
	char i = 0;

	Mat4x4f perspective;
	Vec3f pos;

	Vec3f sun;

	Vec3f color[3];

	Vec3f vertex(const unsigned char *data)
	{
		Vec4f v;
		char j = 0;
		v.cords[0] = readFromBytes<float>(data + (sizeof(float) * (j++)));
		v.cords[1] = readFromBytes<float>(data + (sizeof(float) * (j++)));
		v.cords[2] = readFromBytes<float>(data + (sizeof(float) * (j++)));
		v.cords[3] = 1;
		
		Vec2f uv;
		uv.cords[0] = readFromBytes<float>(data + (sizeof(float) * (j++)));
		uv.cords[1] = readFromBytes<float>(data + (sizeof(float) * (j++)));

		Vec3f normal;
		normal.cords[0] = readFromBytes<float>(data + (sizeof(float) * (j++)));
		normal.cords[1] = readFromBytes<float>(data + (sizeof(float) * (j++)));
		normal.cords[2] = readFromBytes<float>(data + (sizeof(float) * (j++)));

		color[i] = dot(normal, sun)*.5 +.5;

		if (++i == 3) {
			i = 0;
		}

		v += Vec4f(pos[0], pos[1], pos[2], 0);

		v = v * perspective;

		v.cords[0] /= v.cords[3];
		v.cords[1] /= v.cords[3];
		v.cords[2] /= v.cords[3];

		return Vec3f(v.cords[0], v.cords[1], v.cords[2]);
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
	BMP image = { 1024, 1024 };

	size_t image_size = image.widht * image.height;

	image.bit_map = new unsigned char[image_size*3];

	Model model = loadModel("res/model.obj");

	MainShader *shader = new MainShader();

	shader->pos = Vec3f(0, 0, -1);
	shader->sun = normalize(Vec3f(.5, 1, .25));

	Mat4x4f perspective = Mat4x4f(
		Vec4f(1, 0, 0, 0),
		Vec4f(0, 1, 0, 0),
		Vec4f(0, 0, 1, -.25),
		Vec4f(0, 0, 0, 1)
	);
	shader->perspective = perspective;

	initDGL();
	bindShader(shader);
	bindBitmap(image.bit_map, image.widht, image.height);

	setDepthTestFunction( [](const float &input, const float&current) {return input > current;} );

	clearColor({.3, .5, .7, 1});
	clearDepthBuffer(-INFINITY);

	drawElements((unsigned char*)(&model.data[0]), (uint32_t*)(&model.indices[0]), model.indices.size(), 8*sizeof(float));

	writeBMP(image, "out/test.bmp");

	std::cout << "success!" << std::endl;
	//getchar();

	bmp_terminate(image);
	terminateDGL();

	return 0;
}
