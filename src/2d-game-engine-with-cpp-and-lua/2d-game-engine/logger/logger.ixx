module;

#include <iostream>
#include <io.h>
#include <fcntl.h>

export module engine:log;
import std;
import :win32;

namespace
{
	class Console
	{
	public:
		Console()
		{
			if (not Win32::AllocConsole())
				throw std::runtime_error{"Failed to allocate console"};
			freopen_s(&consoleOut, "CONOUT$", "w", stdout);

			// Enable virtual terminal processing so we can use ANSI escape codes for colored output.
			auto consoleHandle = Win32::GetStdHandle(Win32::StdOutputHandle);
			if (consoleHandle == Win32::InvalidHandleValue)
				throw std::runtime_error{"Failed to get console handle"};
			auto consoleMode = DWORD{ 0 };
			if (not Win32::GetConsoleMode(consoleHandle, &consoleMode))
				throw std::runtime_error{ "Failed to get console mode" };
			Win32::SetConsoleMode(consoleHandle, consoleMode | Win32::EnableVirtualTerminalProcessing);
		}
		~Console()
		{
			fclose(consoleOut);
			Win32::FreeConsole();
		}
	private:
		FILE* consoleOut = nullptr;
	};
	const auto console = Console{};
}

namespace Log
{
	template<typename...TArgs>
	void Info(std::format_string<TArgs...> message, TArgs&&... args)
	{
		auto now = std::chrono::system_clock::now();
		std::println("[{}] \x1b[32m[INFO]\x1b[0m {}", std::chrono::zoned_time{ std::chrono::current_zone(), now }, std::format(message, std::forward<TArgs>(args)...));
	}
	template<typename...TArgs>
	void Warn(std::format_string<TArgs...> message, TArgs&&... args)
	{
		auto now = std::chrono::system_clock::now();
		std::println("[{}] \x1b[33m[WARNING]\x1b[0m {}", std::chrono::zoned_time{ std::chrono::current_zone(), now }, std::format(message, std::forward<TArgs>(args)...));
	}
	template<typename...TArgs>
	void Error(std::format_string<TArgs...> message, TArgs&&... args)
	{
		auto now = std::chrono::system_clock::now();
		std::println("[{}] \x1b[31m[ERROR]\x1b[0m {}", std::chrono::zoned_time{ std::chrono::current_zone(), now }, std::format(message, std::forward<TArgs>(args)...));
	}
}
