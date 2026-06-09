export module engine:win32.error;
import std;
import :concepts;
import :win32.exports;

namespace Win32
{
	auto GetErrorMessage(Concepts::OneOf<DWORD, HRESULT> auto&& errorCode) noexcept -> std::string
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
			: std::runtime_error{ GetErrorMessage(errorCode) }
			, code{ errorCode }
		{}
		auto Code() const noexcept -> DWORD { return code; }
	private:
		DWORD code{};
	};

	struct HResult
	{
		HRESULT value{};
		
		explicit HResult(HRESULT hr) noexcept
			: value{ hr }
		{}

		constexpr auto Succeeded(this auto&& self) noexcept -> bool
		{
			return ::Win32::Succeeded(self.value);
		}

		constexpr auto Failed(this auto&& self) noexcept -> bool
		{
			return ::Win32::Failed(self.value);
		}

		constexpr operator bool(this auto&& self) noexcept
		{
			return self.Succeeded();
		}

		constexpr auto operator==(const Win32::HResult& other) const noexcept -> bool
		{
			return value == other.value;
		}
	};

	class ComError : public std::runtime_error
	{
	public:
		explicit ComError(HRESULT hr)
			: std::runtime_error{ GetErrorMessage(hr) }
			, result{ hr }
		{}
		auto Result() const noexcept -> HRESULT { return result; }
	private:
		HRESULT result{};
	};
}
