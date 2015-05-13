#ifndef ZPOGIF_EXCEPTIONS_HPP
#define ZPOGIF_EXCEPTIONS_HPP

#include <stdexcept>
#include "zpogif_error.h"

namespace zpogif {
	
	class gif_exception : public std::runtime_error
	{
		public:
			inline explicit gif_exception(const std::string& what, zpogif_error error):
				std::runtime_error(what),
				_error(error)
			{}
			
			inline zpogif_error error()
			{
				return _error;
			}
		
		private:
			zpogif_error _error;
	};

	class io_error : public gif_exception
	{
		public:
			inline explicit io_error():
				gif_exception("IO error", ZPOGIF_IO_ERROR)
			{}
	};

	class out_of_memory : public gif_exception
	{
		public:
			inline explicit out_of_memory():
				gif_exception("image allocator out of memory", ZPOGIF_OUT_OF_MEMORY)
			{}
	};

	class unexpected_block : public gif_exception
	{
		public:
			inline explicit unexpected_block():
				gif_exception("unexpected block", ZPOGIF_UNEXPECTED_BLOCK)
			{}
	};

	class unexpected_end_of_block : public gif_exception
	{
		public:
			inline explicit unexpected_end_of_block():
				gif_exception("unexpected end of block", ZPOGIF_UNEXPECTED_END_OF_BLOCK)
			{}
	};

	class unexpected_end_of_file : public gif_exception
	{
		public:
			inline explicit unexpected_end_of_file():
				gif_exception("unexpected end of file", ZPOGIF_UNEXPECTED_END_OF_FILE)
			{}
	};

	class invalid_header : public gif_exception
	{
		public:
			inline explicit invalid_header():
				gif_exception("invalid header", ZPOGIF_INVALID_HEADER)
			{}
	};

	class invalid_separator : public gif_exception
	{
		public:
			inline explicit invalid_separator():
				gif_exception("invalid separator", ZPOGIF_INVALID_SEPARATOR)
			{}
	};

	class invalid_extension_length : public gif_exception
	{
		public:
			inline explicit invalid_extension_length():
				gif_exception("invalid extension length", ZPOGIF_INVALID_EXTENSION_LENGTH)
			{}
	};

	class invalid_extension_terminator : public gif_exception
	{
		public:
			inline explicit invalid_extension_terminator():
				gif_exception("invalid extension terminator", ZPOGIF_INVALID_EXTENSION_TERMINATOR)
			{}
	};

	class invalid_extension_label : public gif_exception
	{
		public:
			inline explicit invalid_extension_label():
				gif_exception("invalid extension label", ZPOGIF_INVALID_EXTENSION_LABEL)
			{}
	};

	class no_color_table : public gif_exception
	{
		public:
			inline explicit no_color_table():
				gif_exception("no color table was defined", ZPOGIF_NO_COLOR_TABLE)
			{}
	};

	class malformed_image_data : public gif_exception
	{
		public:
			inline explicit malformed_image_data():
				gif_exception("malformed image data", ZPOGIF_MALFORMED_IMAGE_DATA)
			{}
	};

	class animation_not_supported : public gif_exception
	{
		public:
			inline explicit animation_not_supported():
				gif_exception("animation not supported", ZPOGIF_ANIMATION_NOT_SUPPORTED)
			{}
	};
}

#endif 
