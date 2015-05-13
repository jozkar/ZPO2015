#include <cstdlib>
#include <cstdint>
#include <fstream>
#include <cstddef>
#include <iostream>

void* allocator(uint16_t width, uint16_t height, void* priv, ptrdiff_t* color_stride, ptrdiff_t* row_stride)
{
	std::cout << "Allocating " << width << "x" << height << " image" << std::endl;
	*color_stride = 3;
	*row_stride = width * 3;
	return malloc(width * height * 3);
}

void deallocator(void* image, uint16_t width, uint16_t height, void* priv)
{
	free(image);
}

int main()
{/*
	std::ifstream is("in.gif", std::ofstream::binary);
	std::ofstream os("out.gif", std::ofstream::binary);
	
	void* image;
	uint16_t width;
	uint16_t height;
	ptrdiff_t color_stride;
	ptrdiff_t row_stride;
	gif_load(is, &image, &width, &height, &color_stride, &row_stride, allocator, deallocator, nullptr);
	std::cout << std::dec << "GIF loaded: " << width << "x" << height << ", strides: " << color_stride << ", " << row_stride << std::endl;
	gif_save(os, image, width, height, color_stride, row_stride);*/
}

