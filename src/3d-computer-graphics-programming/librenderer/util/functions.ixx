export module librenderer:functions;
import std;
import :win32;
import :sdl;

export namespace util
{
    inline consteval auto is_debug() noexcept -> bool
    {
#ifdef _DEBUG
        return true;
#else
        return false;
#endif // _DEBUG
    }

    inline consteval auto is_release() noexcept -> bool
    {
        return not is_debug();
    }

    template<typename...TArgs>
    inline auto print_debug_string(std::format_string<TArgs...> fmt, TArgs&&...args) -> std::string
    {
        auto error = std::format("{}\n", std::format(fmt, std::forward<TArgs>(args)...));
        win32::OutputDebugStringA(error.c_str());
        return error;
    }

    auto print_last_error(const std::source_location location = std::source_location::current()) -> std::string
    {
        return print_debug_string(
            "SDL failed [{}] at {}:{}:{}\n",
            sdl::SDL_GetError(),
            location.file_name(),
            location.function_name(),
            location.line()
        );
    }
}
