#ifndef COMMON_HPP
#define COMMON_HPP

#include <istream>
#include <cstdint>
#include <vector>
#include "streamops.hpp"
#include "zpogif_exceptions.hpp"

namespace zpogif { namespace detail {
	
	struct Rgb
	{
		uint8_t r, g, b;
		
		explicit inline Rgb():
			r(0), g(0), b(0)
		{}
		
		explicit inline Rgb(uint8_t gray):
			r(gray), g(gray), b(gray)
		{}
		
		explicit inline Rgb(uint8_t r, uint8_t g, uint8_t b):
			r(r), g(g), b(b)
		{}
		
		bool operator==(Rgb other) const;
		bool operator!=(Rgb other) const;
		bool operator<(Rgb other) const;
	};

	static_assert(sizeof(Rgb) == 3, "struct Rgb must be 3 bytes long");

	struct image_descriptor
	{
		uint16_t left;
		uint16_t top;
		uint16_t width;
		uint16_t height;
		bool has_local_color_table;
		bool interlace_flag;
		bool sort_flag;
		uint8_t local_color_table_size;
		
		std::vector<Rgb> local_color_table;
		
		template<typename T>
		void read(T& is)
		{
			left = io::from_little_endian(io::read<uint16_t>(is));
			top = io::from_little_endian(io::read<uint16_t>(is));
			width = io::from_little_endian(io::read<uint16_t>(is));
			height = io::from_little_endian(io::read<uint16_t>(is));
			
			uint8_t flags = io::read<uint8_t>(is);
			has_local_color_table = flags & 0x80;
			interlace_flag = flags & 0x40;
			sort_flag = flags & 0x20;
			local_color_table_size = flags & 0x7;
			
			if (has_local_color_table)
			{
				auto local_color_table_length = 1 << (local_color_table_size + 1);
				local_color_table.resize(local_color_table_length);
				io::read_array(is, local_color_table.data(), local_color_table_length);
			}
		}
	};

	struct graphic_control_extension
	{
		static constexpr uint8_t label = 0xf9;
		
		uint16_t delay_time;
		uint8_t packed;
		uint8_t transparent_color_index;
		
		inline bool get_transparent_color_flag()
		{
			return packed & 0x1;
		}
		
		inline bool get_user_input_flag()
		{
			return packed & 0x2;
		}
		
		inline uint8_t get_disposal_method()
		{
			return (packed >> 2) & 0x7;
		}
		
		template <typename T>
		void read(T& is)
		{
			if (io::read<uint8_t>(is) != 4) throw zpogif::invalid_extension_length();
			packed = io::read<uint8_t>(is);
			delay_time = io::from_little_endian(io::read<uint16_t>(is));
			transparent_color_index = io::read<uint8_t>(is);
			if (io::read<uint8_t>(is) != 0) throw zpogif::invalid_extension_terminator();
		}
	};

	struct application_extension
	{
		static constexpr uint8_t label = 0xff;
		
		template <typename T>
		void ignore(T& is)
		{
			if (io::read<uint8_t>(is) != 11) throw zpogif::invalid_extension_length();
			io::skip_bytes(is, 11);
			io::skip_blocks(is);
		}
	};

	struct comment_extension
	{
		static constexpr uint8_t label = 0xfe;
		
		template <typename T>
		void ignore(T& is)
		{
			io::skip_blocks(is);
		}
	};

	struct plain_text_extension
	{
		static constexpr uint8_t label = 0x01;
		
		template <typename T>
		void ignore(T& is)
		{
			if (io::read<uint8_t>(is) != 12) throw invalid_extension_length();
			io::skip_bytes(is, 12);
			io::skip_blocks(is);
		}
	};
}}

#endif 
