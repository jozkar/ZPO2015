#ifndef STREAMOPS_HPP
#define STREAMOPS_HPP

#include <iostream>
#include <cstdio>

namespace zpogif { namespace detail { namespace io {
	
	void skip_bytes(std::istream& is, size_t count);
	void skip_bytes(FILE* f, size_t count);

	void skip_blocks(std::istream& is);
	void skip_blocks(FILE* f);

	uint16_t to_little_endian(uint16_t x);
	uint16_t from_little_endian(uint16_t x);
	
	void check_stream(std::ios& stream);
	void check_stream(FILE* stream);
	
	template<typename T>
	T read(std::istream& is)
	{
		T val;
		is.read(reinterpret_cast<char*>(&val), sizeof(T));
		check_stream(is);
		return val;
	}

	template<typename T>
	T read(FILE* f)
	{
		T val;
		fread(&val, sizeof(T), 1, f);
		check_stream(f);
		return val;
	}

	template<typename T>
	void read_array(std::istream& is, T* val, size_t count)
	{
		is.read(reinterpret_cast<char*>(val), count * sizeof(T));
		check_stream(is);
	}

	template<typename T>
	void read_array(FILE* f, T* val, size_t count)
	{
		fread(val, sizeof(T), count, f);
		check_stream(f);
	}

	template<typename T>
	void write(std::ostream& os, const T& val)
	{
		os.write(reinterpret_cast<const char*>(&val), sizeof(T));
		check_stream(os);
	}

	template<typename T>
	void write(FILE* f, const T& val)
	{
		fwrite(&val, sizeof(T), 1, f);
		check_stream(f);
	}

	template<typename T>
	void write_array(std::ostream& os, const T* val, uint32_t count)
	{
		os.write(reinterpret_cast<const char*>(val), sizeof(T) * count);
		check_stream(os);
	}

	template<typename T>
	void write_array(FILE* f, const T* val, uint32_t count)
	{
		fwrite(val, sizeof(T), count, f);
		check_stream(f);
	}
}}}

#endif
