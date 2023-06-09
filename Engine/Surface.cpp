#include "Surface.h"
#include "ChiliWin.h"
#include <fstream>
#include <cassert>

Surface::Surface(int in_width, int in_height)
	:
	width(in_width),
	height(in_height),
	pPixel(new Color[width*height])
{
}

Surface::Surface(const std::string& filename)
{
	std::ifstream file(filename, std::ios::binary);

	assert(file);

	BITMAPFILEHEADER bmFileHeader;
	file.read(reinterpret_cast<char*>(&bmFileHeader), sizeof(bmFileHeader));

	BITMAPINFOHEADER bmInfoHeader;
	file.read(reinterpret_cast<char*>(&bmInfoHeader), sizeof(bmInfoHeader));

	assert(bmInfoHeader.biBitCount == 24 || bmInfoHeader.biBitCount == 32);

	bool is32b = (bmInfoHeader.biBitCount == 32);

	assert(bmInfoHeader.biCompression == BI_RGB);

	width = bmInfoHeader.biWidth;

	int yStart;
	int yEnd;
	int dy;

	if (bmInfoHeader.biHeight < 0) {
		height = -bmInfoHeader.biHeight;
		dy = 1;
		yStart = 0;
		yEnd = height;
	}
	else {
		height = bmInfoHeader.biHeight;
		dy = -1;
		yStart = height - 1;
		yEnd = -1;
	}
	pPixel = new Color[width * height];

	file.seekg(bmFileHeader.bfOffBits);

	const int padding = (4 - (width*3) % 4) % 4;

	for (int y = yStart; y != yEnd; y += dy) {
		for (int x = 0; x < width; x++) {
			PutPixel(x, y,Color(file.get(), file.get(), file.get()));
			if (is32b) {
				file.seekg(1, std::ios::cur);
			}
		}
		if (!is32b) {
			file.seekg(padding, std::ios::cur);
		}
	}

}

Surface::Surface(const Surface& rhs)
	:
	Surface(rhs.width, rhs.height)
{
	const int nPixels = width * height;
	for (int i = 0; i < nPixels; i++) {
		pPixel[i] = rhs.pPixel[i];
	}
}

Surface::~Surface()
{
	delete [] pPixel;
	pPixel = nullptr;
}

Surface& Surface::operator=(const Surface& rhs)
{
	width = rhs.width;
	height = rhs.height;

	delete[] pPixel;
	pPixel = new Color[width * height];
	const int nPixels = width * height;
	for (int i = 0; i < nPixels; i++) {
		pPixel[i] = rhs.pPixel[i];
	}
	return *this;
}

void Surface::PutPixel(int x, int y, Color c)
{
	assert(x >= 0);
	assert(y >= 0);
	assert(x < width);
	assert(y < height);

	pPixel[y * width + x] = c;
}

Color Surface::GetPixel(int x, int y) const
{
	assert(x >= 0);
	assert(y >= 0);
	assert(x < width);
	assert(y < height);

	return pPixel[y * width + x];
}
