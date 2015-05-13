#include "zpogif.hpp"
#include "zpogif.h"
#include "gif-save.hpp"
#include "gif-load.hpp"

namespace zpogif
{
	void save(std::ostream& of, 
		const void* image, 
		uint16_t width, 
		uint16_t height, 
		ptrdiff_t pixel_stride, 
		ptrdiff_t row_stride)
	{
		detail::gif_save<std::ostream&>(of, image, width, height, pixel_stride, row_stride);
	}

	void* load(std::istream& is, 
		uint16_t* width_out,
		uint16_t* height_out,
		ptrdiff_t* pixel_stride_out,
		ptrdiff_t* row_stride_out,
		std::function<void*(uint16_t, uint16_t, ptrdiff_t*, ptrdiff_t*)> allocator,
		std::function<void(uint16_t, uint16_t, void*)> deallocator)
	{
		return detail::gif_load<std::istream&>(is, width_out, height_out, pixel_stride_out, row_stride_out, allocator, deallocator);
	}
	
	void save(FILE* f, 
		const void* image, 
		uint16_t width, 
		uint16_t height, 
		ptrdiff_t pixel_stride, 
		ptrdiff_t row_stride)
	{
		detail::gif_save(f, image, width, height, pixel_stride, row_stride);
	}

	void* load(FILE* f,
		uint16_t* width_out,
		uint16_t* height_out,
		ptrdiff_t* pixel_stride_out,
		ptrdiff_t* row_stride_out,
		std::function<void*(uint16_t, uint16_t, ptrdiff_t*, ptrdiff_t*)> allocator,
		std::function<void(uint16_t, uint16_t, void*)> deallocator)
	{
		return detail::gif_load(f, width_out, height_out, pixel_stride_out, row_stride_out, allocator, deallocator);
	}
}

extern "C"
{
	zpogif_error zpogif_save(FILE* f, 
		const void* image, 
		uint16_t width, 
		uint16_t height, 
		ptrdiff_t pixel_stride, 
		ptrdiff_t row_stride)
	{
		try
		{
			zpogif::save(
				f,
				image,
				width,
				height,
				pixel_stride,
				row_stride);
			
			return ZPOGIF_NO_ERROR;
		}
		catch (zpogif::gif_exception& ex)
		{
			return ex.error();
		}
		catch (...)
		{
			return ZPOGIF_UNIDENTIFIED_ERROR;
		}
	}

	zpogif_error zpogif_load(FILE* f, 
		void** image_out, 
		uint16_t* width_out,
		uint16_t* height_out,
		ptrdiff_t* pixel_stride_out,
		ptrdiff_t* row_stride_out,
		void* (*allocator)(uint16_t, uint16_t, ptrdiff_t*, ptrdiff_t*, void*),
		void (*deallocator)(uint16_t, uint16_t, void*, void*),
		void* allocator_data)
	{
		try
		{
			*image_out = zpogif::load(
				f,
				width_out,
				height_out,
				pixel_stride_out,
				row_stride_out,
				[=](uint16_t width, uint16_t height, ptrdiff_t* pixel_stride_out, ptrdiff_t* row_stride_out) {
					return allocator(width, height, pixel_stride_out, row_stride_out, allocator_data);
				},
				[=](uint16_t width, uint16_t height, void* image) {
					return deallocator(width, height, image, allocator_data);
				});
			
			return ZPOGIF_NO_ERROR;
		}
		catch (zpogif::gif_exception& ex)
		{
			return ex.error();
		}
		catch (...)
		{
			return ZPOGIF_UNIDENTIFIED_ERROR;
		}
	}
}
