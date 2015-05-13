#include "streamops.hpp"

#include <cstdint>
#include <cstdio>
#include "zpogif_exceptions.hpp"

namespace zpogif { namespace detail { namespace io {
	
	void skip_bytes(std::istream& is, size_t count)
	{
		is.ignore(count);
		check_stream(is);
	}
	
	void skip_bytes(FILE* f, size_t count)
	{
		fseek(f, count, SEEK_CUR);
		check_stream(f);
	}
	
	void skip_blocks(std::istream& is)
	{
		while (true)
		{
			uint8_t block_length = read<uint8_t>(is);
			if (block_length == 0) break;
			skip_bytes(is, block_length);
		}
	}

	void skip_blocks(FILE* f)
	{
		while (true)
		{
			uint8_t block_length = read<uint8_t>(f);
			if (block_length == 0) break;
			skip_bytes(f, block_length);
		}
	}
	
	uint16_t from_little_endian(uint16_t x)
	{
		uint8_t* ptr = reinterpret_cast<uint8_t*>(&x);
		return ptr[0] | (ptr[1] << 8);
	}

	uint16_t to_little_endian(uint16_t x)
	{
		uint8_t bytes[2];
		bytes[0] = x & 0xff;
		bytes[1] = (x >> 8) & 0xff;
		return *reinterpret_cast<uint16_t*>(bytes);
	}
	
	void check_stream(std::ios& stream)
	{
		if (!stream.good()) throw zpogif::io_error();
	}
	
	void check_stream(FILE* stream)
	{
		if (feof(stream) || ferror(stream)) throw zpogif::io_error();
	}
}}}
