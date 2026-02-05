module;

#include "upng.h"

export module librenderer:lib.upng;

export namespace upng
{
	using
		::upng_error,
		::upng_format,
		::upng_t,
		::upng_new_from_bytes,
		::upng_new_from_file,
		::upng_free,
		::upng_get_error,
		::upng_get_format,
		::upng_get_width,
		::upng_get_height,
		::upng_decode,
		::upng_get_buffer
		;
}
