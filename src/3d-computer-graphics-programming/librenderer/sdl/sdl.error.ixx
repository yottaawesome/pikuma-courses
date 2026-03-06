export module librenderer:sdl.error;
import std;
import :util;
import :sdl.exports;

export namespace sdl
{
    auto print_last_error(const std::source_location location = std::source_location::current()) -> std::string
    {
        return util::print_debug_string(
            "SDL failed [{}] at {}:{}:{}\n",
            sdl::SDL_GetError(),
            location.file_name(),
            location.function_name(),
            location.line()
        );
    }

    class error final : public std::runtime_error
    {
    public:
        error(std::string_view sdl_error, std::string_view message, const std::source_location location = std::source_location::current())
            : std::runtime_error(std::format("SDL error: {} - {} at {}:{}:{}\n", sdl_error, message, location.file_name(), location.function_name(), location.line()))
        { }
    };
}
