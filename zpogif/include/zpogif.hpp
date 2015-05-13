#ifndef ZPOGIF_HPP
#define ZPOGIF_HPP

#include <cstddef>
#include <iostream>
#include <functional>
#include "zpogif_exceptions.hpp"
#include "zpogif_format.h"

namespace zpogif {
	
	void save(std::ostream& of, 
		zpogif_format format,
		const void* image, 
		uint16_t width, 
		uint16_t height, 
		ptrdiff_t pixel_stride, 
		ptrdiff_t row_stride);

	void* load(std::istream& is,
		zpogif_format format,
		uint16_t* width_out,
		uint16_t* height_out,
		ptrdiff_t* pixel_stride_out,
		ptrdiff_t* row_stride_out,
		std::function<void*(zpogif_format, uint16_t, uint16_t, ptrdiff_t*, ptrdiff_t*)> allocator,
		std::function<void(zpogif_format, uint16_t, uint16_t, void*)> deallocator);
	
	void save(FILE* f, 
		zpogif_format format,
		const void* image, 
		uint16_t width, 
		uint16_t height, 
		ptrdiff_t pixel_stride, 
		ptrdiff_t row_stride);

	void* load(FILE* f,
		zpogif_format format,
		uint16_t* width_out,
		uint16_t* height_out,
		ptrdiff_t* pixel_stride_out,
		ptrdiff_t* row_stride_out,
		std::function<void*(zpogif_format, uint16_t, uint16_t, ptrdiff_t*, ptrdiff_t*)> allocator,
		std::function<void(zpogif_format, uint16_t, uint16_t, void*)> deallocator);
}

#endif
