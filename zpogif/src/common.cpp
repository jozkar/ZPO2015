#include <iostream>
#include "common.hpp"
#include "zpogif_exceptions.hpp"
#include "streamops.hpp"

namespace zpogif { namespace detail {

	bool Rgb::operator==(Rgb other) const
	{
		return r == other.r && g == other.g && b == other.b;
	}

	bool Rgb::operator!=(Rgb other) const
	{
		return !(*this == other);
	}

	bool Rgb::operator<(Rgb other) const
	{
		return r < other.r || (r == other.r && (g < other.g || (g == other.g && b < other.b)));
	}
}}
