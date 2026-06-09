module;

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

export module engine:win32.exports;

export namespace Win32
{
	template<auto Value>
	struct Constant
	{
		using Type = decltype(Value);
		static constexpr auto operator()() noexcept -> Type
		{
			return Value;
		}

		constexpr operator Type(this auto&&) noexcept
		{
			return Value;
		}
	};

	using
		::HANDLE,
		::HMODULE,
		::HINSTANCE,
		::LPWSTR,
		::DWORD,
		::HRESULT,
		::LPCSTR,
		::CloseHandle,
		::LocalFree,
		::FormatMessageA,
		::GetLastError,
		::AllocConsole,
		::FreeConsole,
		::MessageBoxA,
		::SetConsoleMode,
		::GetStdHandle,
		::GetConsoleMode
		;

	constexpr auto EnableVirtualTerminalProcessing = ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	constexpr auto StdOutputHandle = STD_OUTPUT_HANDLE;
	constexpr auto InvalidHandleValue = Constant<INVALID_HANDLE_VALUE>{};

	enum MessageBoxFlags
	{
		OK = MB_OK,
		IconError = MB_ICONERROR
	};

	enum FormatMessageFlags
	{
		AllocateBuffer = FORMAT_MESSAGE_ALLOCATE_BUFFER,
		FromSystem = FORMAT_MESSAGE_FROM_SYSTEM,
		IgnoreInserts = FORMAT_MESSAGE_IGNORE_INSERTS
	};

	constexpr auto Succeeded(HRESULT hr) noexcept -> bool
	{
		return SUCCEEDED(hr);
	}
	constexpr auto Failed(HRESULT hr) noexcept -> bool
	{
		return FAILED(hr);
	}
}
