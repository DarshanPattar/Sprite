#pragma once

#include "Colors.h"
#include <string>

class Surface {
public:
	Surface(int in_width, int in_height);
	Surface(const std::string& filename);
	Surface(const Surface& rhs);
	~Surface();

	Surface& operator=(const Surface&);
	void PutPixel(int x, int y, Color c);
	Color GetPixel(int x, int y) const;

	inline int GetWidth() const { return width; }
	inline int GetHeight() const { return height; }
private:
	Color* pPixel;
	int width;
	int height;
};