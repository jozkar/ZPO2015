#ifndef ZPOGIF_H
#define ZPOGIF_H

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include "zpogif_error.h"
#include "zpogif_format.h"

#ifdef __cplusplus
extern "C" {
#endif

	zpogif_error zpogif_save(FILE* f,
		zpogif_format format,
		const void* image, 
		uint16_t width, 
		uint16_t height, 
		ptrdiff_t pixel_stride, 
		ptrdiff_t row_stride);

	zpogif_error zpogif_load(FILE* f,
		zpogif_format format,
		void** image_out, 
		uint16_t* width_out,
		uint16_t* height_out,
		ptrdiff_t* pixel_stride_out,
		ptrdiff_t* row_stride_out,
		void* (*allocator)(zpogif_format, uint16_t, uint16_t, ptrdiff_t*, ptrdiff_t*, void*),
		void (*deallocator)(zpogif_format, uint16_t, uint16_t, void*, void*),
		void* allocator_data);

#ifdef __cplusplus
}
#endif

#endif
