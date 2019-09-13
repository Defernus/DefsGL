#include "model.h"

#include <fstream>
#include <string>
#include <sstream>
#include <math.h>

Model loadModel(const char* obj_path)
{
	std::string line;
	std::ifstream file;
	file.open(obj_path);

	std::vector<float> vertices;
	std::vector<float> normals;
	std::vector<float> uvs;

	Model model;

	uint32_t i = 0;

	while (std::getline(file, line))
	{
		if (line.size() < 3 || line[0] == '#')continue;


		std::istringstream iss(line);

		std::string type = "";

		iss >> type;

		if (type == "v")
		{
			double a, b, c;
			iss >> a >> b >> c;
			vertices.push_back(a);
			vertices.push_back(b);
			vertices.push_back(c);
			continue;
		}
		if (type == "vn")
		{
			double a, b, c;
			iss >> a >> b >> c;
			normals.push_back(a);
			normals.push_back(b);
			normals.push_back(c);
			continue;
		}

		if (type == "vt")
		{
			double a, b, c;
			iss >> a >> b;
			uvs.push_back(a);
			uvs.push_back(b);
			continue;
		}

		if (type == "f")
		{
			int va, vb, vc;
			int vta, vtb, vtc;
			int vna, vnb, vnc;
			char skip;
			iss >> va >> skip >> vta >> skip >> vna >> vb >> skip >> vtb >> skip >> vnb >> vc >> skip >> vtc >> skip >> vnc;

			--va;
			--vb;
			--vc;

			--vta;
			--vtb;
			--vtc;

			--vna;
			--vnb;
			--vnc;

			model.indices.push_back(i * 3);
			model.indices.push_back(i * 3 + 1);
			model.indices.push_back((i++) * 3 + 2);


			model.data.push_back(vertices[va * 3]);
			model.data.push_back(vertices[va * 3 + 1]);
			model.data.push_back(vertices[va * 3 + 2]);

			model.data.push_back(uvs[vta * 2]);
			model.data.push_back(uvs[vta * 2 + 1]);

			model.data.push_back(normals[vna * 3]);
			model.data.push_back(normals[vna * 3 + 1]);
			model.data.push_back(normals[vna * 3 + 2]);


			model.data.push_back(vertices[vb * 3]);
			model.data.push_back(vertices[vb * 3 + 1]);
			model.data.push_back(vertices[vb * 3 + 2]);

			model.data.push_back(uvs[vtb * 2]);
			model.data.push_back(uvs[vtb * 2 + 1]);

			model.data.push_back(normals[vnb * 3]);
			model.data.push_back(normals[vnb * 3 + 1]);
			model.data.push_back(normals[vnb * 3 + 2]);


			model.data.push_back(vertices[vc * 3]);
			model.data.push_back(vertices[vc * 3 + 1]);
			model.data.push_back(vertices[vc * 3 + 2]);

			model.data.push_back(uvs[vtc * 2]);
			model.data.push_back(uvs[vtc * 2 + 1]);

			model.data.push_back(normals[vnc * 3]);
			model.data.push_back(normals[vnc * 3 + 1]);
			model.data.push_back(normals[vnc * 3 + 2]);

			continue;
		}
	}
	file.close();

	return model;
}