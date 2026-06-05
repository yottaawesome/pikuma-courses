module;

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

export module engine:win32.exports;

export namespace Win32
{
	using
		::HANDLE,
		::HINSTANCE,
		::LPWSTR,
		::MessageBoxA
		;

	enum MessageBoxFlags
	{
		OK = MB_OK,
		IconError = MB_ICONERROR
	};
}
