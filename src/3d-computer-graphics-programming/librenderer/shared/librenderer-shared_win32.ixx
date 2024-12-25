module;

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

export module librenderer:shared_win32;

export namespace win32
{
	using
		::OutputDebugStringA,
		::OutputDebugStringW
		;
}