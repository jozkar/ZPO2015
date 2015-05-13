#ifndef GIF_LOAD_HPP
#define GIF_LOAD_HPP

#include <cstring>
#include "streamops.hpp"
#include "zpogif_format.h"

namespace zpogif { namespace detail {
	
	// dělí bajty na kódy
	template<typename T>
	struct code_reader
	{
		code_reader(T& stream):
			code_size(0),
			stream(stream),
			current_offset(0),
			block_length(0),
			block_position(0)
		{}
		
		uint8_t code_size;
		
		bool read(uint16_t& result)
		{
			result = 0;
			int remaining_code_size = code_size;
			
			while (remaining_code_size != 0)
			{
				if (block_length == block_position)
				{
					block_length = io::read<uint8_t>(stream);
					if (block_length == 0) // jsme na konci dat?
					{
						if (remaining_code_size != code_size) throw unexpected_end_of_block();
						else return false;
					}
					
					io::read_array(stream, buffer.data(), block_length);
					block_position = 0;
				}
				
				int bit_count = std::min(8 - current_offset, remaining_code_size);
				assert(bit_count != 0);
				auto current_byte = buffer[block_position];
				
				auto masked = (current_byte >> current_offset) & ((1 << bit_count) - 1);
				result |= masked << (code_size - remaining_code_size);
				
				current_offset += bit_count;
				remaining_code_size -= bit_count;
				
				assert(current_offset <= 8);
				
				if (current_offset == 8)
				{
					block_position++;
					current_offset = 0;
				}
			}
			
			return true;
		}
		
		void finish()
		{
			if (io::read<uint8_t>(stream) == 0) return;
			else throw malformed_image_data();
		}
		
	private:
		T& stream;
		
		std::array<uint8_t, 255> buffer;
		int current_offset;
		int block_length;
		int block_position;
	};

	struct image_allocator_sentry
	{
		inline image_allocator_sentry(
			void* image,
			uint16_t width,
			uint16_t height,
			std::function<void(uint16_t, uint16_t, void*)> deallocator):
			image(image),
			width(width),
			height(height),
			deallocator(deallocator)
		{}
		
		inline ~image_allocator_sentry()
		{
			if (image != nullptr)
			{
				deallocator(width, height, image);
			}
		}

		inline void disable()
		{
			image = nullptr;
		}

	private:
		void* image;
		uint16_t width;
		uint16_t height;
		std::function<void(uint16_t, uint16_t, void*)> deallocator;
	};

	inline uint8_t to_grayscale(Rgb color)
	{
		return (299 * color.r + 587 * color.g + 114 * color.b) / 1000;
	}

	inline void write_color(void* ptr, zpogif_format format, Rgb color)
	{
		if (format == ZPOGIF_RGB)
			*reinterpret_cast<Rgb*>(ptr) = color;
		else
			*reinterpret_cast<uint8_t*>(ptr) = to_grayscale(color);
	}

	template <typename T>
	void* gif_load(T is, 
		zpogif_format format,
		uint16_t* width_out,
		uint16_t* height_out,
		ptrdiff_t* pixel_stride_out,
		ptrdiff_t* row_stride_out,
		std::function<void*(uint16_t, uint16_t, ptrdiff_t*, ptrdiff_t*)> allocator,
		std::function<void(uint16_t, uint16_t, void*)> deallocator)
	{
		{
			char header[6];
			io::read_array(is, header, 6);
			if (memcmp(header, "GIF89a", 6) != 0 && memcmp(header, "GIF87a", 6) != 0) throw invalid_header();
		}
		
		uint16_t width = io::from_little_endian(io::read<uint16_t>(is));
		uint16_t height = io::from_little_endian(io::read<uint16_t>(is));
		
		*width_out = width;
		*height_out = height;
		
		uint8_t flags = io::read<uint8_t>(is);
		bool has_global_color_table = flags & 0x80;
		uint8_t global_color_table_size = flags & 0x7;
		auto global_color_table_length = has_global_color_table ? 1 << (global_color_table_size + 1) : 0;
		uint8_t global_background_color = io::read<uint8_t>(is);
		io::read<uint8_t>(is); // pixel aspect ratio
		
		ptrdiff_t pixel_stride;
		ptrdiff_t row_stride;
		
		void* image = allocator(width, height, &pixel_stride, &row_stride);
		*pixel_stride_out = pixel_stride;
		*row_stride_out = row_stride;
		if (image == nullptr) throw out_of_memory();
		image_allocator_sentry image_sentry(image, width, height, deallocator);
		
		std::vector<Rgb> global_color_table(global_color_table_length);
		if (has_global_color_table)
		{
			io::read_array(is, global_color_table.data(), global_color_table_length);
		}
		
		if (has_global_color_table)
		{
			for (unsigned y = 0; y < height; y++)
			{
				uint8_t* row_ptr = reinterpret_cast<uint8_t*>(image) + y * row_stride;
				for (unsigned x = 0; x < width; x++)
				{
					uint8_t* pixel_ptr = row_ptr + x * pixel_stride;
					write_color(pixel_ptr, format, global_color_table.at(global_background_color));
				}
			}
		}
		
		bool has_gcext = false;
		graphic_control_extension gcext;
		
		while (true)
		{
			uint8_t separator = io::read<uint8_t>(is);
		
			image_descriptor idesc;
			application_extension aext;
			comment_extension cext;
			plain_text_extension ptext;
			
			if (separator == 0x21) // extension
			{
				uint8_t label = io::read<uint8_t>(is);
				
				switch (label)
				{
					case graphic_control_extension::label:
						if (has_gcext) throw unexpected_block();
						gcext.read(is);
						has_gcext = true;
						break;
					
					case application_extension::label:
						if (has_gcext) throw unexpected_block();
						aext.ignore(is);
						break;
					
					case comment_extension::label:
						if (has_gcext) throw unexpected_block();
						cext.ignore(is);
						break;
					
					case plain_text_extension::label:
						ptext.ignore(is);
						has_gcext = false;
						break;
					
					default:
						throw invalid_extension_label();
				}
			}
			else if (separator == 0x2c) // image descriptor
			{
				idesc.read(is);
				
				uint8_t minimum_code_size = io::read<uint8_t>(is);
				code_reader<T> reader(is);
				reader.code_size = minimum_code_size + 1;
				uint16_t cc = (1 << minimum_code_size);
				uint16_t eoi = cc + 1;
				uint16_t next_code = eoi + 1;
				
				if (!has_global_color_table && !idesc.has_local_color_table) throw no_color_table();
				
				std::vector<Rgb>& color_table = idesc.has_local_color_table ? idesc.local_color_table : global_color_table;
				std::map<uint16_t, std::vector<uint8_t>> code_table;
				
				int imx = 0;
				int imy = 0;
				
				auto add_pixel = [&](uint8_t index){
					if (imy == idesc.height) throw malformed_image_data();
					
					uint8_t* imptr = reinterpret_cast<uint8_t*>(image);
					uint8_t* pixelptr = imptr + (idesc.top + imy) * row_stride + (idesc.left + imx) * pixel_stride;
					write_color(pixelptr, format, color_table.at(index));
					
					imx++;
					if (imx == idesc.width)
					{
						imx = 0;
						imy++;
					}
				};
				
				
				// následuje LZW dekomprese
				bool is_first_code = true;
				uint16_t code, previous_code;
				while (reader.read(code))
				{
					if (code == cc)
					{
						code_table.clear();
						reader.code_size = minimum_code_size + 1;
						for (unsigned i = 0; i < cc; i++) code_table[i] = std::vector<uint8_t>(1, i);
						is_first_code = true;
						continue;
					}
					else if (code == eoi)
					{
						break;
					}
					
					if (is_first_code)
					{
						add_pixel(code);
						is_first_code = false;
					}
					else
					{
						auto code_in_table = code_table.find(code);
						if (code_in_table != code_table.end())
						{
							auto& idx_stream = code_in_table->second;
							for (auto idx : idx_stream) add_pixel(idx);
							
							auto prev_stream = code_table.at(previous_code);
							prev_stream.push_back(idx_stream[0]);
							code_table[next_code++] = prev_stream;
						}
						else
						{
							auto prev_stream = code_table.at(previous_code);
							prev_stream.push_back(prev_stream[0]);
							for (auto idx : prev_stream) add_pixel(idx);
							code_table[next_code++] = prev_stream;
						}
						
						if (next_code == (1 << reader.code_size)) reader.code_size++;
					}
					
					previous_code = code;
				}
				reader.finish();
				has_gcext = false;
			}
			else if (separator == 0x3B) // trailer
			{
				break;
			}
			else
			{
				throw invalid_separator();
			}
		}
		
		image_sentry.disable();
		
		return image;
	}
}}

#endif
