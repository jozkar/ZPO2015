#ifndef ZPOGIF_HPP
#define ZPOGIF_HPP

#include <cstddef>
#include <iostream>
#include <functional>
#include "zpogif_exceptions.hpp"

namespace zpogif {
	
	void save(std::ostream& of, 
		const void* image, 
		uint16_t width, 
		uint16_t height, 
		ptrdiff_t pixel_stride, 
		ptrdiff_t row_stride);

	void* load(std::istream& is, 
		uint16_t* width_out,
		uint16_t* height_out,
		ptrdiff_t* pixel_stride_out,
		ptrdiff_t* row_stride_out,
		std::function<void*(uint16_t, uint16_t, ptrdiff_t*, ptrdiff_t*)> allocator,
		std::function<void(uint16_t, uint16_t, void*)> deallocator);
	
	void save(FILE* f, 
		const void* image, 
		uint16_t width, 
		uint16_t height, 
		ptrdiff_t pixel_stride, 
		ptrdiff_t row_stride);

	void* load(FILE* f,
		uint16_t* width_out,
		uint16_t* height_out,
		ptrdiff_t* pixel_stride_out,
		ptrdiff_t* row_stride_out,
		std::function<void*(uint16_t, uint16_t, ptrdiff_t*, ptrdiff_t*)> allocator,
		std::function<void(uint16_t, uint16_t, void*)> deallocator);
}

#endif
