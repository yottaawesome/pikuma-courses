export module engine:win32.error;
import std;
import :win32.exports;

namespace Win32
{
	auto GetLastErrorMessage(DWORD errorCode) noexcept -> std::string
	{
		if (errorCode == 0)
			return "No error";
		constexpr auto flags = 
			FormatMessageFlags::AllocateBuffer 
			| FormatMessageFlags::FromSystem 
			| FormatMessageFlags::IgnoreInserts;
		auto messageBuffer = LPSTR{};
		auto size = FormatMessageA(
			flags,
			nullptr,
			errorCode,
			0,
			reinterpret_cast<LPSTR>(&messageBuffer),
			0,
			nullptr
		);
		if (size == 0)
			return "Unknown error";
		auto string = std::string{messageBuffer};
		LocalFree(messageBuffer);
		while (string.back() == '\n' or string.back() == '\r')
			string.pop_back();
		return string;
	}
}

export namespace Win32
{
	class Win32Error : public std::runtime_error
	{
	public:
		explicit Win32Error(DWORD errorCode)
			: std::runtime_error{ GetLastErrorMessage(errorCode) }
			, code{ errorCode }
		{}
		auto Code() const noexcept -> DWORD { return code; }
	private:
		DWORD code{};
	};
}
