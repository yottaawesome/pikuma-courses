export module Engine:SDL3.Error;
import std;
import :SDL3.Exports;

export namespace SDL
{
	class SdlError : public std::runtime_error
	{
	public:
		explicit SdlError()
			: std::runtime_error{SDL_GetError()} 
		{}
		explicit SdlError(std::string_view message)
			: std::runtime_error{ std::format("{}: {}", message, SDL_GetError()) }
		{}
	};
}