#pragma once

/**
 *simple BMP image r/w header
 *
 *
 *
 *EXAMPLE:
 *
 *    BMP image = {256, 256};
 *    
 *    size_t image_size = image.widht * image.height;
 *    image.bit_map = new unsigned char[image_size*3];
 *    
 *    unsigned x = 0;
 *    unsigned y = 0;
 *    
 *    for (unsigned char *i = image.bit_map; i != image.bit_map + (image_size * 3); i += 3)
 *    {
 *        if (x++ == image.widht)
 *        {
 *            x = 0;
 *            ++y;
 *            if (y == image.height) y = 0;
 *        }
 *        
 *        i[0] = x;
 *        i[1] = y;
 *        i[2] = (x+y)%256;
 *    }
 *    
 *    write(image, "test.bmp");
 *    
 *    bmp_terminate(image);
 *
 *
 *
 *made by Defernus for fun
 */

#include <fstream>
#include <iostream>

struct BMP
{
	uint32_t widht, height;
	
	unsigned char *bit_map;
};

void bmp_terminate(BMP &bmp)
{
	delete[] bmp.bit_map;
}

template<typename T>
T readFromBytes(const unsigned char *bytes)
{
	union
	{
		T v;
		unsigned char b[sizeof(T)];
	}a;

	for (unsigned char *i = (unsigned char*)bytes, *j = a.b; i != bytes + sizeof(T); ++i, ++j)
	{
		*j = *i;
	}

	return a.v;
}

template<typename T>
void writeToFile(const T &var, std::ofstream &file)
{
	union
	{
		T v;
		char b[sizeof(T)];
	}a;
	a.v = var;

	file.write(a.b, sizeof(T));
}

/*incomplite*/
BMP load(const char *path)
{
	BMP bmp;

	std::ifstream file;
	file.open(path, std::ifstream::binary);

	file.seekg(0, file.end);
	int length = file.tellg();
	file.seekg(0, file.beg);

	bmp.bit_map = new unsigned char[length];

	file.read((char*)bmp.bit_map, length);

	file.close();

	if (bmp.bit_map[0] != 0x42 || bmp.bit_map[1] != 0x4D)throw "wrong file format (type error)";

	if (readFromBytes<uint32_t>(bmp.bit_map + 2) != length) throw "wrong file format (length error)";

	uint32_t offset_bits = readFromBytes<uint32_t>(bmp.bit_map + 0x0A);

	bmp.widht = readFromBytes<uint32_t>(bmp.bit_map + 0x12);
	bmp.height = readFromBytes<uint32_t>(bmp.bit_map + 0x16);

	uint16_t bitCount = readFromBytes<uint16_t>(bmp.bit_map + 0x1A);



	return bmp;
}

void write(const BMP &bmp, const char *path)
{
	std::ofstream file;
	file.open(path, std::ofstream::binary);

	uint32_t byte_count = 3;
	uint16_t bit_count = byte_count*8;
	uint32_t off_bits = 1078;
	uint32_t image_size = byte_count * bmp.widht*bmp.height;
	uint32_t file_size = image_size + off_bits;

	writeToFile(uint16_t(0x4D42), file);
	writeToFile(file_size, file);
	writeToFile(uint32_t(0x0000), file);
	writeToFile(off_bits, file);

	writeToFile(uint32_t(40), file);
	writeToFile(bmp.widht, file);
	writeToFile(bmp.height, file);

	writeToFile(uint16_t(1), file);//planes

	writeToFile(bit_count, file);

	writeToFile(uint32_t(0), file);//compression

	writeToFile(image_size, file);

	writeToFile(bmp.widht, file);
	writeToFile(bmp.widht, file);

	writeToFile(uint32_t(0), file);
	writeToFile(uint32_t(0), file);

	char blank[1024];
	for (char *i = blank; i != blank + 1024; ++i)
	{
		*i = 0;
	}

	file.write(blank, 1024);

	file.write((char*)bmp.bit_map, image_size);

	file.close();
}

