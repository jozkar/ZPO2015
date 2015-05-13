#include "zpogif.hpp"
#include "zpogif.h"
#include "gif-save.hpp"
#include "gif-load.hpp"

namespace zpogif
{
	void save(std::ostream& of, 
		zpogif_format format,
		const void* image, 
		uint16_t width, 
		uint16_t height, 
		ptrdiff_t pixel_stride, 
		ptrdiff_t row_stride)
	{
		detail::gif_save<std::ostream&>(of, format, image, width, height, pixel_stride, row_stride);
	}

	void* load(std::istream& is, 
		zpogif_format format,
		uint16_t* width_out,
		uint16_t* height_out,
		ptrdiff_t* pixel_stride_out,
		ptrdiff_t* row_stride_out,
		std::function<void*(zpogif_format, uint16_t, uint16_t, ptrdiff_t*, ptrdiff_t*)> allocator,
		std::function<void(zpogif_format, uint16_t, uint16_t, void*)> deallocator)
	{
		return detail::gif_load<std::istream&>(is, format, width_out, height_out, pixel_stride_out, row_stride_out, allocator, deallocator);
	}
	
	void save(FILE* f, 
		zpogif_format format,
		const void* image, 
		uint16_t width, 
		uint16_t height, 
		ptrdiff_t pixel_stride, 
		ptrdiff_t row_stride)
	{
		detail::gif_save(f, format, image, width, height, pixel_stride, row_stride);
	}

	void* load(FILE* f,
		zpogif_format format,
		uint16_t* width_out,
		uint16_t* height_out,
		ptrdiff_t* pixel_stride_out,
		ptrdiff_t* row_stride_out,
		std::function<void*(zpogif_format, uint16_t, uint16_t, ptrdiff_t*, ptrdiff_t*)> allocator,
		std::function<void(zpogif_format, uint16_t, uint16_t, void*)> deallocator)
	{
		return detail::gif_load(f, format, width_out, height_out, pixel_stride_out, row_stride_out, allocator, deallocator);
	}
}

extern "C"
{
	zpogif_error zpogif_save(FILE* f, 
		zpogif_format format,
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
				format,
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
		zpogif_format format,
		void** image_out, 
		uint16_t* width_out,
		uint16_t* height_out,
		ptrdiff_t* pixel_stride_out,
		ptrdiff_t* row_stride_out,
		void* (*allocator)(zpogif_format, uint16_t, uint16_t, ptrdiff_t*, ptrdiff_t*, void*),
		void (*deallocator)(zpogif_format, uint16_t, uint16_t, void*, void*),
		void* allocator_data)
	{
		try
		{
			*image_out = zpogif::load(
				f,
				format,
				width_out,
				height_out,
				pixel_stride_out,
				row_stride_out,
				[=](zpogif_format format, uint16_t width, uint16_t height, ptrdiff_t* pixel_stride_out, ptrdiff_t* row_stride_out) {
					return allocator(format, width, height, pixel_stride_out, row_stride_out, allocator_data);
				},
				[=](zpogif_format format, uint16_t width, uint16_t height, void* image) {
					return deallocator(format, width, height, image, allocator_data);
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
