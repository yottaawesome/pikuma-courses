export module engine:win32.util;
import std;
import :win32.exports;

export namespace Win32
{
	void ShowError(const std::string& message) noexcept
	{
		MessageBoxA(nullptr, message.c_str(), "Error", MessageBoxFlags::OK | MessageBoxFlags::IconError);
	}
}
