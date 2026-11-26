export module Engine:Win32.Util;
import std;
import :Win32.Exports;

export namespace Win32
{
	void ShowError(const std::string& message) noexcept
	{
		MessageBoxA(nullptr, message.c_str(), "Error", MessageBoxFlags::OK | MessageBoxFlags::IconError);
	}
}
