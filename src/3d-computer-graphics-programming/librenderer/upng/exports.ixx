module;

// If using .c extension, it will use C linkage and cause 
// linker errors when we try to link to it from C++ code.
// We can either wrap this in extern "C" (which generates 
// compiler warnings about this being in the global 
// module fragment) or compile it as C++, which is cleaner.
#include "upng.hpp"

export module librenderer:upng.exports;
import std;
import :raii;

export using
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