#ifndef GIF_SAVE_HPP
#define GIF_SAVE_HPP

#include <cassert>
#include <set>
#include <vector>
#include <map>
#include "common.hpp"
#include "streamops.hpp"
#include "zpogif_format.h"

namespace zpogif { namespace detail {
	void clusterize(const std::set<Rgb>& colors, std::map<Rgb, uint8_t>& idxmap, std::vector<Rgb>& color_table);
	
	// spojuje bity do bajtu a vklada znacky delky bloku
	template<typename T>
	struct code_writer
	{
		code_writer(T stream):
			code_size(0),
			stream(stream),
			current_byte(0),
			current_offset(0)
		{}
		
		uint8_t code_size;
		
		void write(uint16_t code)
		{
			int remaining_code_size = code_size;
			
			while (remaining_code_size != 0)
			{
				int bit_count = std::min(8 - current_offset, remaining_code_size);
				assert(bit_count != 0);
				auto masked = (((1 << bit_count) - 1) & code);
				current_byte |= masked << current_offset;
				code >>= bit_count;
				
				current_offset += bit_count;
				remaining_code_size -= bit_count;
				
				assert(current_offset <= 8);
				
				if (current_offset == 8)
				{
					// dokoncili jsme bajt
					buffer.push_back(current_byte);
					current_byte = 0;
					current_offset = 0;
					
					if (buffer.size() == 255)
					{
						// dokoncili jsme blok
						finish_buffer();
					}
				}
			}
		}
		
		void finish()
		{
			if (current_offset != 0)
			{
				buffer.push_back(current_byte);
			}
			
			finish_buffer();
			uint8_t terminator = 0;
			io::write(stream, terminator);
		}
		
	private:
		void finish_buffer()
		{
			assert(buffer.size() <= 255);
			uint8_t size = buffer.size();
			io::write(stream, size);
			io::write_array(stream, buffer.data(), size);
			buffer.clear();
		}

		T stream;
		uint8_t current_byte;
		int current_offset;
		std::vector<uint8_t> buffer;
	};

	Rgb read_color(const void* ptr, zpogif_format format)
	{
		if (format == ZPOGIF_RGB)
		{
			return *reinterpret_cast<const Rgb*>(ptr);
		}
		else
		{
			return Rgb(*reinterpret_cast<const uint8_t*>(ptr));
		}
	}

	template<typename T>
	void gif_save(T of, 
		zpogif_format format,
		const void* image, 
		uint16_t width, 
		uint16_t height, 
		ptrdiff_t pixel_stride, 
		ptrdiff_t row_stride)
	{
		io::write_array(of, "GIF89a", 6);
		uint16_t width_le = io::to_little_endian(width);
		uint16_t height_le = io::to_little_endian(height);
		io::write(of, width_le);
		io::write(of, height_le);
		
		std::set<Rgb> colors;
		
		for (uint32_t y = 0; y < height; y++)
		{
			const uint8_t* row_ptr = reinterpret_cast<const uint8_t*>(image) + row_stride * y;
			for (uint32_t x = 0; x < width; x++)
			{
				Rgb color = read_color(row_ptr + pixel_stride * x, format);
				colors.insert(color);
			}
		}
		
		std::map<Rgb, uint8_t> color_table;
		std::vector<Rgb> colors_vec;
		
		clusterize(colors, color_table, colors_vec);
		
		uint8_t color_table_size = 
			colors_vec.size() <= 2 ? 0 
			: colors_vec.size() <= 4 ? 1
			: colors_vec.size() <= 8 ? 2
			: colors_vec.size() <= 16 ? 3
			: colors_vec.size() <= 32 ? 4
			: colors_vec.size() <= 64 ? 5
			: colors_vec.size() <= 128 ? 6
			: 7;
		
		uint8_t flags = 0xf0 | color_table_size;
		io::write(of, flags);
		
		uint16_t todo = 0;
		io::write(of, todo);
		
		// vypiseme globalni tabulku barev
		for (auto color : colors_vec)
		{
			io::write(of, color);
		}
		
		// doplnime zbytek globalni tabulky barev
		Rgb filler{255,255,255};
		for (auto i = colors_vec.size(); i < (1u << (color_table_size + 1)); i++)
		{
			io::write(of, filler);
		}
		
		// vypiseme image descriptor
		uint8_t image_descriptor_separator = 0x2c;
		io::write(of, image_descriptor_separator);
		uint16_t left = 0, top = 0;
		io::write(of, left);
		io::write(of, top);
		io::write(of, width_le);
		io::write(of, height_le);
		uint8_t image_descriptor_flags = 0x00;
		io::write(of, image_descriptor_flags);
		
		// zde zacina LZW komprese
		std::map<std::vector<uint8_t>, uint16_t> code_table;
		std::vector<uint8_t> buffer;
		
		uint8_t lzw_min_code_size = color_table_size + 1;
		uint16_t cc = (1 << lzw_min_code_size);
		uint16_t eoi = cc + 1;
		uint16_t next_code = eoi + 1;
		
		io::write(of, lzw_min_code_size);
		code_writer<T> writer(of);
		writer.code_size = lzw_min_code_size + 1;
		writer.write(cc);
		
		for (uint32_t y = 0; y < height; y++)
		{
			const uint8_t* row_ptr = reinterpret_cast<const uint8_t*>(image) + row_stride * y;
			for (uint32_t x = 0; x < width; x++)
			{
				Rgb color = read_color(row_ptr + pixel_stride * x, format);
				uint8_t idx = color_table.at(color);
				
				if (x == 0 && y == 0)
				{
					// inicializace bufferu
					buffer.push_back(idx);
				}
				else
				{
					buffer.push_back(idx);
					auto idx_in_table = code_table.find(buffer);
					if (idx_in_table == code_table.end())
					{
						code_table[buffer] = next_code++;
						
						buffer.pop_back();
						uint16_t emitted_code = buffer.size() == 1 ? buffer[0] : code_table.at(buffer);
						writer.write(emitted_code);
						
						if (next_code - 1 == (1 << writer.code_size))
						{
							writer.code_size++;
						}
						
						buffer.clear();
						
						if (next_code == 4096) // limit 12 bitů na kód
						{
							code_table.clear();
							writer.write(cc);
							writer.code_size = lzw_min_code_size + 1;
							next_code = eoi + 1;
						}
						
						buffer.push_back(idx);
					}
				}
			}
		}
		
		{
			uint8_t final_emitted_code = buffer.size() == 1 ? buffer[0] : code_table.at(buffer);
			writer.write(final_emitted_code);
			writer.write(eoi);
			writer.finish();
		}
		
		uint8_t trailer = 0x3b;
		io::write(of, trailer);
	}
}}

#endif
